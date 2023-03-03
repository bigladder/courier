/* Copyright (c) 2019 Big Ladder Software LLC. All rights reserved.
 * See the LICENSE file for additional terms and conditions. */

#include <gtest/gtest.h>
#include <courierr/courierr.h>
#include <fmt/format.h>

class TestCourierr : Courierr::CourierrBase {
  public:
    virtual void error(const std::string_view message)
    {
        std::cerr << fmt::format("[ERROR] {}", message) << std::endl;
        throw Courierr::CourierrException(message);
    }
    virtual void warning(const std::string_view message) { write_message("WARNING", message); }
    virtual void info(const std::string_view message) { write_message("INFO", message); }
    virtual void debug(const std::string_view message) { write_message("DEBUG", message); }

  private:
    void write_message(const std::string_view message_type, const std::string_view message)
    {
        std::cout << fmt::format("[{}] {}", message_type, message) << std::endl;
    }
};

TEST(CourierrBase, Warning)
{
    TestCourierr courier;
    courier.warning("This is a warning.");
}

TEST(CourierrBase, Error)
{
    TestCourierr courier;
    EXPECT_THROW(courier.error("This is an error!"), Courierr::CourierrException);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
