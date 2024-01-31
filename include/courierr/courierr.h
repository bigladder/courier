/* Copyright (c) 2019 Big Ladder Software LLC. All rights reserved.
 * See the LICENSE file for additional terms and conditions. */

#pragma once

#include <string>
#include <iostream>

namespace Courierr {

class Courierr {
  public:
    virtual ~Courierr() = default;

    // Methods to be used in library source code
    [[noreturn]] virtual void error(const std::string& message) final
    {
        error_override(message);
        throw std::runtime_error("Courierr: Error not handled by derived class: \"" + message +
                                 "\"");
    }
    virtual void warning(const std::string& message) final { warning_override(message); }
    virtual void info(const std::string& message) final { info_override(message); }
    virtual void debug(const std::string& message) final { debug_override(message); }

  protected:
    // Virtual methods to be overridden by derived class
    [[noreturn]] virtual void error_override(const std::string& message) = 0;
    virtual void warning_override(const std::string& message) = 0;
    virtual void info_override(const std::string& message) = 0;
    virtual void debug_override(const std::string& message) = 0;
};

} // namespace Courierr
