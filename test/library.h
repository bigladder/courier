#pragma once

#include <string>
#include <utility>
#include <courier/courier.h>
#include <courier/helpers.h>
#include <fmt/format.h>

class LibraryClass : Courier::Dispatcher {
  public:
    explicit LibraryClass(std::string name_in,
                          const std::shared_ptr<Courier::Courier>& courier_in =
                              std::make_shared<Courier::DefaultCourier>())
        : Courier::Dispatcher(std::move(name_in), courier_in)
    {
        class_name = "LibraryClass";
    }
    void generate_debug() { send_debug("Something for the developer happened."); }
    void generate_info() { send_info("Something you should notice happened."); }
    void generate_warning() { send_warning("Something unexpected happened."); }
    void generate_error() { send_error("Something serious happened."); }
    void set_courier(std::shared_ptr<Courier::Courier> courier_in)
    {
        courier = std::move(courier_in);
    }
    std::shared_ptr<Courier::Courier> get_courier() { return courier; }
};
