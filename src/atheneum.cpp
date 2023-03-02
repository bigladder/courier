/* Copyright (c) 2019 Big Ladder Software LLC. All rights reserved.
 * See the LICENSE file for additional terms and conditions. */

#include <memory>

#include <fmt/format.h>

#include <atheneum/atheneum.h>

#include "atheneum-private.h"
#include "logging-base.h"

namespace Atheneum {

Atheneum::Atheneum(LoggingCallbackFunction logging_callback_function_,
                   void* logging_callback_context_)
{
    atheneum = std::unique_ptr<AtheneumPrivate>(
        new AtheneumPrivate(logging_callback_function_, logging_callback_context_));
}

Atheneum::~Atheneum() {}

int Atheneum::answer() { return atheneum->answer(); }

AtheneumPrivate::AtheneumPrivate(LoggingCallbackFunction logging_callback_function_,
                                 void* logging_callback_context_)
    : LoggingBase(logging_callback_function_, logging_callback_context_)
{
}

int AtheneumPrivate::answer()
{
    int answer = 42;
    log_message(
        MessageLevel::MESSAGE_INFO,
        fmt::format(
            "The answer to the ultimate question of life, the universe, and everything is: {}.",
            answer));
    return answer;
}

} // namespace Atheneum
