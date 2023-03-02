/* Copyright (c) 2019 Big Ladder Software LLC. All rights reserved.
 * See the LICENSE file for additional terms and conditions. */

#include <iostream>
#include <map>

#include <fmt/format.h>

#include "logging-base.h"

namespace Atheneum {

static std::map<const MessageLevel, const std::string_view> message_map {
    {MessageLevel::MESSAGE_ERROR, "ERROR"},
    {MessageLevel::MESSAGE_WARNING, "WARNING"},
    {MessageLevel::MESSAGE_INFO, "INFO"},
    {MessageLevel::MESSAGE_DEBUG, "DEBUG"},
};

void default_logging_callback_function(const MessageLevel message_level,
                                       const std::string_view message,
                                       void*)
{
    std::string full_message = fmt::format("  {}: {}", message_map[message_level], message);
    if (message_level == MessageLevel::MESSAGE_ERROR) {
        std::cerr << full_message << std::endl;
        throw AtheneumException(message);
    }
    else {
        std::cout << full_message << std::endl;
    }
}

LoggingBase::LoggingBase(LoggingCallbackFunction logging_callback_function_,
                         void* logging_callback_context_)
    : logging_callback_function(logging_callback_function_)
    , logging_callback_context(logging_callback_context_)
{
    if (!logging_callback_function) {
        logging_callback_function = default_logging_callback_function;
    }
}

void LoggingBase::log_message(MessageLevel message_level, std::string_view message)
{
    logging_callback_function(message_level, message, logging_callback_context);
}

} // namespace Atheneum
