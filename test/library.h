#pragma once

#include <string>
#include <utility>
#include <courierr/courierr.h>
#include <fmt/format.h>

class LibraryDefaultCourier : public Courierr::Courierr {
    void receive_error(const std::string& message) override { write_message("ERROR", message); }
    void receive_warning(const std::string& message) override { write_message("WARNING", message); }
    void receive_info(const std::string& message) override { write_message("INFO", message); }
    void receive_debug(const std::string& message) override { write_message("DEBUG", message); }
    virtual void write_message(const std::string& message_type, const std::string& message)
    {
        std::cout << fmt::format("[{}] {}", message_type, message) << std::endl;
        std::cout
            << "  Generated using LibraryDefaultCourier. Consider deriving your own Courier class!"
            << std::endl;
    }
};

class LibraryClass {
  public:
    LibraryClass(std::string name_in,
                 const std::shared_ptr<Courierr::Courierr>& courier_in =
                     std::make_shared<LibraryDefaultCourier>())
        : name(std::move(name_in)), courier(courier_in)
    {
    }
    void generate_debug()
    {
        courier->send_debug(make_message("Something for the developer happened"));
    }
    void generate_info()
    {
        courier->send_info(make_message("Something you should notice happened"));
    }
    void generate_warning()
    {
        courier->send_warning(make_message("Something unexpected happened"));
    }
    void generate_error() { courier->send_error(make_message("Something serious happened")); }
    void set_courier(std::shared_ptr<Courierr::Courierr> courier_in)
    {
        courier = std::move(courier_in);
    }
    std::shared_ptr<Courierr::Courierr> get_courier() { return courier; }

  private:
    std::string name;
    std::shared_ptr<Courierr::Courierr> courier;
    static constexpr std::string_view message_format {"LibraryClass({}): {}"};
    std::string make_message(const std::string& message)
    {
        return fmt::format(message_format, name, message);
    }
};
