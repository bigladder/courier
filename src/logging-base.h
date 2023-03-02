/* Copyright (c) 2019 Big Ladder Software LLC. All rights reserved.
 * See the LICENSE file for additional terms and conditions. */

#pragma once

#include <functional>
#include <string_view>
#include <string>

namespace Atheneum {

enum class MessageLevel { MESSAGE_DEBUG, MESSAGE_INFO, MESSAGE_WARNING, MESSAGE_ERROR };
using LoggingCallbackFunction = std::function<void(MessageLevel, const std::string_view, void*)>;

class AtheneumException : public std::exception {
  public:
    explicit AtheneumException(const char* message_) : message(message_) {}
    explicit AtheneumException(const std::string& message_) : message(message_) {}
    explicit AtheneumException(const std::string_view message_) : message(message_) {}
    virtual ~AtheneumException() noexcept = default;
    virtual const char* what() const noexcept { return message.c_str(); }

  protected:
    std::string message;
};

class LoggingBase {
  public:
    LoggingBase(LoggingCallbackFunction logging_callback_function_ = nullptr,
                void* logging_callback_context_ = nullptr);
    void log_message(MessageLevel message_level, std::string_view message);
    void set_logging_callback_function(LoggingCallbackFunction logging_callback_function_);
    void set_logging_callback_context(void* logging_callback_context_);

  private:
    LoggingCallbackFunction logging_callback_function {nullptr};
    void* logging_callback_context {nullptr};
};

} // namespace Atheneum
