/* Copyright (c) 2019 Big Ladder Software LLC. All rights reserved.
 * See the LICENSE file for additional terms and conditions. */

#pragma once

#include <string>
#include <iostream>

namespace Courier {

class Courier {
  public:
    virtual ~Courier() = default;

    // Sending methods to be used in library source code
    [[noreturn]] virtual void send_error(const std::string& message) final
    {
        receive_error(message);
        throw std::runtime_error("Courier: Error not handled by derived class: \"" + message +
                                 "\"");
    }
    virtual void send_warning(const std::string& message) final { receive_warning(message); }
    virtual void send_info(const std::string& message) final { receive_info(message); }
    virtual void send_debug(const std::string& message) final { receive_debug(message); }

  protected:
    // Virtual receiving methods to be overridden by derived class
    [[noreturn]] virtual void receive_error(const std::string& message) = 0;
    virtual void receive_warning(const std::string& message) = 0;
    virtual void receive_info(const std::string& message) = 0;
    virtual void receive_debug(const std::string& message) = 0;
};

} // namespace Courier
