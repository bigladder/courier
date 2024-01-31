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

TEST(Courierr, Warning)
{
    ClientClass client_object("My object");
    std::string expected_output {"[WARNING] ClientClass(My object) LibraryClass(My library "
                                 "object): Something unexpected happened\n"};
    EXPECT_STDOUT(client_object.make_library_warning();, expected_output)
}

TEST(Courierr, Error)
{
    ClientClass client_object("My object");
    std::string expected_output {"[ERROR] ClientClass(My object) LibraryClass(My library object): "
                                 "Something serious happened\n"};
    EXPECT_STDOUT(EXPECT_THROW(client_object.make_library_error(), std::runtime_error);
                  , expected_output)
}

TEST(Courierr, Minimal)
{
    class MinimalCourier : public Courierr::Courierr {
        void error_override(const std::string& message) override {}
        void warning_override(const std::string& message) override {}
        void info_override(const std::string& message) override {}
        void debug_override(const std::string& message) override {}
    };

    ClientClass client_object("My object");
    client_object.library_object.set_courier(std::make_shared<MinimalCourier>());
    try {
        client_object.make_library_error();
    }
    catch (std::runtime_error& exception) {
        EXPECT_STREQ(exception.what(),
                     "Courierr: Error not handled by derived class: \"LibraryClass(My library "
                     "object): Something serious happened\"");
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
