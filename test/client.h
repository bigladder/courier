#pragma once

#include "library.h"

class ClientClass {
  public:
    ClientClass(std::string name_in);
    std::string name;
    LibraryClass library_object;
    void make_library_warning() { library_object.method_with_warning(); }
    void make_library_error() { library_object.method_with_error(); }
};

class SimpleBaseCourier : public Courierr::Courierr {
  public:
    explicit SimpleBaseCourier() = default;
    void error_override(const std::string& message) override
    {
        write_message("ERROR", message);
        throw std::runtime_error(message);
    }
    void warning_override(const std::string& message) override
    {
        write_message("WARNING", message);
    }
    void info_override(const std::string& message) override { write_message("INFO", message); }
    void debug_override(const std::string& message) override { write_message("DEBUG", message); }

  protected:
    virtual void write_message(const std::string& message_type, const std::string& message) = 0;
};

class ClientCourier : public SimpleBaseCourier {
  public:
    explicit ClientCourier(ClientClass* client_class_pointer_in)
        : client_class_pointer(client_class_pointer_in)
    {
    }

  private:
    ClientClass* client_class_pointer;
    void write_message(const std::string& message_type, const std::string& message) override
    {
        std::string context_format =
            client_class_pointer ? fmt::format(" ClientClass({})", client_class_pointer->name) : "";
        std::cout << fmt::format("[{}]{} {}", message_type, context_format, message) << std::endl;
    }
};

ClientClass::ClientClass(std::string name_in)
    : name(name_in), library_object("My library object", std::make_shared<ClientCourier>(this))
{
}