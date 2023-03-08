/* Copyright (c) 2019 Big Ladder Software LLC. All rights reserved.
 * See the LICENSE file for additional terms and conditions. */

#pragma once

#include <functional>
#include <string_view>
#include <string>

#include <fmt/format.h>

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

class SimpleCourierr : public Courierr::CourierrBase {
  public:
    void error(const std::string_view message) override { write_message("ERROR", message); }
    void warning(const std::string_view message) override { write_message("WARNING", message); }
    void info(const std::string_view message) override { write_message("INFO", message); }
    void debug(const std::string_view message) override { write_message("DEBUG", message); }

  private:
    void write_message(const std::string_view message_type, const std::string_view message)
    {
        std::cout << fmt::format("[{}] {}", message_type, message) << std::endl;
    }
};

class CourierrException : public std::exception {
  public:
    explicit CourierrException(const char* message_, CourierrBase& logger)
     : message(message_)
     , courierr(logger)
    {
      write_error();
    }
    explicit CourierrException(const std::string& message_, CourierrBase& logger)
     : message(message_)
     , courierr(logger)
    {
      write_error();
    }
    explicit CourierrException(const std::string_view message_, CourierrBase& logger)
     : message(message_)
     , courierr(logger)
    {
      write_error();
    }

    virtual ~CourierrException() noexcept = default;
    virtual const char* what() const noexcept { return message.c_str(); }

  protected:
    std::string message;

  private:
    CourierrBase& courierr;
    void write_error() { courierr.error(message); } 
};

} // namespace Courierr
