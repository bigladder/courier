#pragma once

#include <string>
#include <courierr/courierr.h>
#include <fmt/format.h>

class LibraryClass {
  public:
    LibraryClass(const std::string& name_in, const std::shared_ptr<Courierr::Courierr>& courier_in)
        : name(name_in), courier(courier_in)
    {
    }

    void method_with_warning()
    {
        courier->warning(fmt::format("LibraryClass({}): Something unexpected happened", name));
    }

    void method_with_error()
    {
        courier->error(fmt::format("LibraryClass({}): Something serious happened", name));
    }
    void set_courier(std::shared_ptr<Courierr::Courierr> courier_in)
    {
        courier = std::move(courier_in);
    }
    std::shared_ptr<Courierr::Courierr> get_courier() { return courier; }

  private:
    std::string name;
    std::shared_ptr<Courierr::Courierr> courier;
};
