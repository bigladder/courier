/* Copyright (c) 2019 Big Ladder Software LLC. All rights reserved.
 * See the LICENSE file for additional terms and conditions. */

#pragma once

#include <fmt/format.h>

#include "courier.h"

namespace Courier {

class DefaultCourier : public Courier {
    void receive_error(const std::string& message) override
    {
        write_message("ERROR", message);
        throw std::runtime_error(message);
    }
    void receive_warning(const std::string& message) override { write_message("WARNING", message); }
    void receive_info(const std::string& message) override { write_message("INFO", message); }
    void receive_debug(const std::string& message) override { write_message("DEBUG", message); }
    virtual void write_message(const std::string& message_type, const std::string& message)
    {
        std::cout << fmt::format("[{}] {}", message_type, message) << std::endl;
        std::cout << "  Generated using Courier::DefaultCourier. Consider deriving your own "
                     "Courier class!"
                  << std::endl;
    }
};

class Dispatcher {
  public:
    Dispatcher() = default;
    explicit Dispatcher(
        std::string name_in,
        const std::shared_ptr<Courier>& courier_in = std::make_shared<DefaultCourier>())
        : name(std::move(name_in)), courier(courier_in) {};
    std::string name;

  protected:
    std::shared_ptr<Courier> courier;
    Dispatcher* parent_pointer {nullptr};
    std::string class_name;
    std::string message_format {"{} '{}': {}"};
    [[nodiscard]] std::string make_message(const std::string& message) const // NOLINT
    {
        std::string local_message = fmt::format(message_format, class_name, name, message);
        if (parent_pointer) {
            return parent_pointer->make_message(local_message);
        }
        return local_message;
    }
    void send_error(const std::string& message) const
    {
        courier->send_error(make_message(message));
    }
    void send_warning(const std::string& message) const
    {
        courier->send_warning(make_message(message));
    }
    void send_info(const std::string& message) const { courier->send_info(make_message(message)); }
    void send_debug(const std::string& message) const
    {
        courier->send_debug(make_message(message));
    }
};

} // namespace Courier
