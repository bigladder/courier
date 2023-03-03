/* Copyright (c) 2019 Big Ladder Software LLC. All rights reserved.
 * See the LICENSE file for additional terms and conditions. */

#pragma once

#include <functional>
#include <string_view>
#include <string>

namespace Courierr {

class CourierrBase {
  public:
    virtual ~CourierrBase() = default;

    virtual void error(const std::string_view message) = 0;
    virtual void warning(const std::string_view message) = 0;
    virtual void info(const std::string_view message) = 0;
    virtual void debug(const std::string_view message) = 0;

    void set_message_context(void* message_context_) { message_context = message_context_; };

  protected:
    void* message_context {nullptr};
};

class CourierrException : public std::exception {
  public:
    explicit CourierrException(const char* message_) : message(message_) {}
    explicit CourierrException(const std::string& message_) : message(message_) {}
    explicit CourierrException(const std::string_view message_) : message(message_) {}
    virtual ~CourierrException() noexcept = default;
    virtual const char* what() const noexcept { return message.c_str(); }

  protected:
    std::string message;
};

} // namespace Courierr
