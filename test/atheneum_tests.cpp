/* Copyright (c) 2019 Big Ladder Software LLC. All rights reserved.
 * See the LICENSE file for additional terms and conditions. */

#include <gtest/gtest.h>
#include <atheneum/atheneum.h>
#include "../src/logging-base.h"

TEST(Atheneum, Example)
{
    Atheneum::Atheneum atheneum;
    EXPECT_EQ(atheneum.answer(), 42);
}

TEST(LoggingBase, Warning)
{
    Atheneum::LoggingBase logger;
    logger.log_message(Atheneum::MessageLevel::MESSAGE_WARNING, "This is a warning.");
}

TEST(LoggingBase, Error)
{
    Atheneum::LoggingBase logger;
    EXPECT_THROW(logger.log_message(Atheneum::MessageLevel::MESSAGE_ERROR, "This is an error!"),
                 Atheneum::AtheneumException);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
