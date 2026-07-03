/* Copyright (c) 2019 Big Ladder Software LLC. All rights reserved.
 * See the LICENSE file for additional terms and conditions. */

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <courier/courier.h>
#include <courier/helpers.h>

namespace {

// A Courier that records the messages it receives so tests can assert on the
// exact strings a Sender subclass produces via make_message().
class RecordingCourier : public Courier::Courier {
  public:
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    std::vector<std::string> infos;
    std::vector<std::string> debugs;

  protected:
    void receive_error(const std::string& message) override
    {
        errors.push_back(message);
        throw std::runtime_error(message);
    }
    void receive_warning(const std::string& message) override { warnings.push_back(message); }
    void receive_info(const std::string& message) override { infos.push_back(message); }
    void receive_debug(const std::string& message) override { debugs.push_back(message); }
};

// A minimal Sender subclass that exposes the protected send_* methods for
// testing.
class BasicSender : public Courier::Sender {
  public:
    explicit BasicSender(std::string name_in, const std::shared_ptr<Courier::Courier>& courier_in)
        : Courier::Sender("BasicSender", std::move(name_in), courier_in)
    {
    }
    void emit_error(const std::string& message) { send_error(message); }
    void emit_warning(const std::string& message) { send_warning(message); }
    void emit_info(const std::string& message) { send_info(message); }
    void emit_debug(const std::string& message) { send_debug(message); }
};

// A Sender subclass that overrides the protected message_format member to change
// how make_message() renders messages.
class CustomFormatSender : public Courier::Sender {
  public:
    explicit CustomFormatSender(std::string name_in,
                                const std::shared_ptr<Courier::Courier>& courier_in)
        : Courier::Sender("CustomFormatSender", std::move(name_in), courier_in)
    {
        message_format = "<{}::{}> {}";
    }
    void emit_warning(const std::string& message) { send_warning(message); }
};

// A Sender subclass used to test parent-pointer message chaining.
class ChildSender : public Courier::Sender {
  public:
    explicit ChildSender(std::string name_in, const std::shared_ptr<Courier::Courier>& courier_in)
        : Courier::Sender("ChildSender", std::move(name_in), courier_in)
    {
    }
    void emit_info(const std::string& message) { send_info(message); }
};

} // namespace

TEST(Sender, SubclassSendsFormattedMessages)
{
    auto courier = std::make_shared<RecordingCourier>();
    BasicSender sender("my sender", courier);

    sender.emit_warning("something happened");
    ASSERT_EQ(courier->warnings.size(), 1u);
    EXPECT_EQ(courier->warnings[0], "BasicSender 'my sender': something happened");

    sender.emit_info("noteworthy");
    ASSERT_EQ(courier->infos.size(), 1u);
    EXPECT_EQ(courier->infos[0], "BasicSender 'my sender': noteworthy");

    sender.emit_debug("dev detail");
    ASSERT_EQ(courier->debugs.size(), 1u);
    EXPECT_EQ(courier->debugs[0], "BasicSender 'my sender': dev detail");

    EXPECT_THROW(sender.emit_error("fatal"), std::runtime_error);
    ASSERT_EQ(courier->errors.size(), 1u);
    EXPECT_EQ(courier->errors[0], "BasicSender 'my sender': fatal");
}

TEST(Sender, GetAndSetCourier)
{
    auto first = std::make_shared<RecordingCourier>();
    BasicSender sender("my sender", first);

    EXPECT_EQ(sender.get_courier(), first);

    auto second = std::make_shared<RecordingCourier>();
    sender.set_courier(second);
    EXPECT_EQ(sender.get_courier(), second);

    // Messages should now be routed to the newly assigned courier.
    sender.emit_info("routed to second");
    EXPECT_TRUE(first->infos.empty());
    ASSERT_EQ(second->infos.size(), 1u);
    EXPECT_EQ(second->infos[0], "BasicSender 'my sender': routed to second");
}

TEST(Sender, OverriddenMessageFormat)
{
    auto courier = std::make_shared<RecordingCourier>();
    CustomFormatSender sender("fmt sender", courier);

    sender.emit_warning("custom layout");
    ASSERT_EQ(courier->warnings.size(), 1u);
    EXPECT_EQ(courier->warnings[0], "<CustomFormatSender::fmt sender> custom layout");
}

TEST(Sender, ParentPointerChainsMessages)
{
    auto courier = std::make_shared<RecordingCourier>();
    BasicSender parent("parent", courier);
    ChildSender child("child", courier);
    child.set_parent_pointer(&parent);

    child.emit_info("nested");
    ASSERT_EQ(courier->infos.size(), 1u);
    EXPECT_EQ(courier->infos[0], "BasicSender 'parent': ChildSender 'child': nested");
}
