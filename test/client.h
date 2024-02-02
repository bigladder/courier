#pragma once

#include "library.h"

class ClientClass {
  public:
    explicit ClientClass(std::string name_in);
    std::string name;
    LibraryClass library_object;
};

class ClientCourier : public Courier::Courier {
  public:
    explicit ClientCourier(ClientClass* client_class_pointer_in)
        : client_class_pointer(client_class_pointer_in)
    {
    }
    enum class MessageLevel { all, debug, info, warning, error };
    MessageLevel message_level {MessageLevel::info};

  protected:
    ClientClass* client_class_pointer;

    void receive_error(const std::string& message) override
    {
        write_message("ERROR", message);
        throw std::runtime_error(message);
    }
    void receive_warning(const std::string& message) override
    {
        if (message_level <= MessageLevel::warning) {
            write_message("WARNING", message);
        }
    }
    void receive_info(const std::string& message) override
    {
        if (message_level <= MessageLevel::info) {
            write_message("INFO", message);
        }
    }
    void receive_debug(const std::string& message) override
    {
        if (message_level <= MessageLevel::debug) {
            write_message("DEBUG", message);
        }
    }
    void write_message(const std::string& message_type, const std::string& message)
    {
        std::string context_format =
            client_class_pointer ? fmt::format(" ClientClass({})", client_class_pointer->name) : "";
        std::cout << fmt::format("[{}]{} {}", message_type, context_format, message) << std::endl;
    }
};

ClientClass::ClientClass(std::string name_in)
    : name(std::move(name_in))
    , library_object("My library object", std::make_shared<ClientCourier>(this))
{
}
