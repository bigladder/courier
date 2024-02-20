/* Copyright (c) 2019 Big Ladder Software LLC. All rights reserved.
 * See the LICENSE file for additional terms and conditions. */

#include <gtest/gtest.h>
#include "client.h"

#define EXPECT_STDOUT(action, ExpectedOut)                                                         \
    {                                                                                              \
        std::stringstream buffer;                                                                  \
        std::streambuf* sbuf = std::cout.rdbuf();                                                  \
        std::cout.rdbuf(buffer.rdbuf());                                                           \
        action std::string capture = buffer.str();                                                 \
        std::cout.rdbuf(sbuf);                                                                     \
        EXPECT_STREQ(ExpectedOut.c_str(), buffer.str().c_str());                                   \
    }

TEST(Courier, Warning)
{
    ClientClass client_object("My object");
    std::string expected_output {"[WARNING] ClientClass 'My object': LibraryClass 'My library "
                                 "object': Something unexpected happened.\n"};
    EXPECT_STDOUT(client_object.library_object.generate_warning();, expected_output)
}

TEST(Courier, Error)
{
    ClientClass client_object("My object");
    std::string expected_output {
        "[ERROR] ClientClass 'My object': LibraryClass 'My library object': "
        "Something serious happened.\n"};
    EXPECT_STDOUT(EXPECT_THROW(client_object.library_object.generate_error(), std::runtime_error);
                  , expected_output)
}

TEST(Courier, MessageLevel)
{
    ClientClass client_object("My object");

    // Debug off by default
    std::string expected_output; // ""
    EXPECT_STDOUT(client_object.library_object.generate_debug();, expected_output)

    // Info on by default
    expected_output = "[INFO] ClientClass 'My object': LibraryClass 'My library object': Something "
                      "you should notice happened.\n";
    EXPECT_STDOUT(client_object.library_object.generate_info();, expected_output)

    // Set message level higher than "info", and confirm info isn't showing up anymore
    std::dynamic_pointer_cast<ClientCourier>(client_object.library_object.get_courier())
        ->message_level = ClientCourier::MessageLevel::warning;
    expected_output = "";
    EXPECT_STDOUT(client_object.library_object.generate_info();, expected_output)

    // Make sure warnings are still showing up
    expected_output = "[WARNING] ClientClass 'My object': LibraryClass 'My library object': "
                      "Something unexpected happened.\n";
    EXPECT_STDOUT(client_object.library_object.generate_warning();, expected_output)
}

TEST(Courier, Minimal)
{
    class MinimalCourier : public Courier::Courier {
        void receive_error(const std::string& message) override {}
        void receive_warning(const std::string& message) override {}
        void receive_info(const std::string& message) override {}
        void receive_debug(const std::string& message) override {}
    };

    ClientClass client_object("My object");
    client_object.library_object.set_courier(std::make_shared<MinimalCourier>());
    try {
        client_object.library_object.generate_error();
    }
    catch (std::runtime_error& exception) {
        EXPECT_STREQ(exception.what(),
                     "Courier error not handled by derived class: LibraryClass 'My library "
                     "object': Something serious happened.");
    }
}

TEST(Courier, Default)
{
    // Test message generated by LibraryDefaultCourier
    LibraryClass new_library_object("My library object");
    std::string expected_output {
        "[DEBUG] LibraryClass 'My library object': Something for the developer happened.\n  "
        "Generated using Courier::DefaultCourier. Consider deriving your own Courier class!\n"};
    EXPECT_STDOUT(new_library_object.generate_debug();, expected_output)
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
