# Courier

A lightweight, header-only C++ library for use in other libraries that allows client code define how to:

1. deliver context-specific error, warning, and informational messages from a library to the client's users, and
2. handle errors from a library when they occur.

Courier itself is NOT a logger or an error handler. It simply provides a mechanism for clients to decide how to log and
handle errors encountered in a library.

## Error Levels

Courier defines four message levels (based
on [Python Logging Levels](https://docs.python.org/3/library/logging.html#logging-levels)):

1. `error`: Due to a more serious problem, the software has not been able to perform some
   function. Errors encountered in library code imply that execution cannot continue reliably within the library.
   Courier allows the client to decide how to handle errors. However, if an error is ultimately not handled,
   Courier will throw an exception from the library.
2. `warning`: An indication that something unexpected happened, or that a problem might occur
   in the near future. The software is still working as expected.
3. `info`: Confirmation that things are working as expected.
4. `debug`: Detailed information, typically only of interest to a developer trying to
   diagnose a problem.

## Usage

Courier has two audiences:

1. Library developers that want to give their clients flexibility in message logging and error handling, and
2. Clients adopting such libraries

### Library Developers

Developers add a `Courier` (though we recommend using a `std::shared_ptr<Courier::Courier>`) data member to any
public class in a library where an error (or other message event) might occur.

```c++
#include <courier/courier.h>

class LibraryClass {
  public:
    LibraryClass(std::string name_in, const std::shared_ptr<Courier::Courier>& courier_in)
        : name(std::move(name_in)), courier(courier_in) {}
  private:
    std::string name;
    std::shared_ptr<Courier::Courier> courier;
};
```

The `Courier::Courier` base class defines an interface with a public methods for each of the four message levels:

- `send_debug(const std::string& message)`
- `send_info(const std::string& message)`
- `send_warning(const std::string& message)`
- `send_error(const std::string& message)`

These methods are called from within the library class. For example:

```c++
if (problem_exists)
{
    courier->send_error("Something terrible happened.");
}
```

#### Good Practices

The following examples illustrate ways to use `Courier` most effectively as a library developer. These practices are
also implemented in [test/library.h](test/library.h).

1. The parent class should provide access to set and get the `Courier` pointer:

    ```c++
    void set_courier(std::shared_ptr<Courier::Courier> courier_in) 
    {
        courier = std::move(courier_in);
    }
    std::shared_ptr<Courier::Courier> get_courier() { return courier; }

   ```

2. By making `Courier` shared pointers and providing access, multiple closely-related objects can share a
   single `Courier` instance.

3. Consider defining default derived `Courier`. Some library users will not care how messages are handled, and don't
   want the additional hassle of developing a derived `Courier` class.

### Library Adopters

Adopters must derive a class from `Courier::Courier`. The `Courier` base class establishes four virtual functions for
receiving events:

- `receive_debug(const std::string& message)`
- `receive_info(const std::string& message)`
- `receive_warning(const std::string& message)`
- `receive_error(const std::string& message)`

#### Good Practices

Consider the following patterns for your derived class(s). These practices are also implemented
in [test/client.h](test/client.h).

1. Create a function to route all messages into a consistent format.

    ```c++
    virtual void make_message(const std::string& message_type, const std::string& message) = 0;
    void receive_error(const std::string& message) override
    {
        make_message("ERROR", message);
        throw std::runtime_error(message);
    }
    void receive_warning(const std::string& message) override { make_message("WARNING", message); }
    void receive_info(const std::string& message) override { make_message("INFO", message); }
    void receive_debug(const std::string& message) override { make_message("DEBUG", message); }
    ```

2. Add your context to your received messages:

    ```c++
    protected:
       ClientClass* client_class_pointer;
       void make_message(const std::string& message_type, const std::string& message) override
       {
           std::string context_format =
               client_class_pointer ? fmt::format(" ClientClass({})", client_class_pointer->name) : "";
           std::cout << fmt::format("[{}]{} {}", message_type, context_format, message) << std::endl;
       }
   ```

3. Add a message level data member to temporarily silence events below a certain message level.

    ```c++
    enum class MessageLevel { all, debug, info, warning, error };
    MessageLevel message_level {MessageLevel::info};
    void receive_warning(const std::string& message) override
    {
        if (message_level <= MessageLevel::warning) {
            make_message("WARNING", message);
        }
    }
    ```