# Courierr

A lightweight, header-only C++ library for use in other libraries that allows client code define how to:

1. deliver context-specific error, warning, and informational messages from a library to the client's users, and
2. handle errors from a library when they occur.

Courrier itself is NOT a logger or an error handler. It simply provides a mechanism for clients to decide how to log and
handle errors encountered in a library.

## Error Levels

Courierr defines four message levels (based
on [Python Logging Levels](https://docs.python.org/3/library/logging.html#logging-levels)):

1. `error`: Due to a more serious problem, the software has not been able to perform some
   function. Errors encountered in library code imply that execution cannot continue reliably within the library.
   Courierr allows the client to decide how to handle errors. However, if an error is ultimately not handled,
   Courrier will throw an exception from the library.
2. `warning`: An indication that something unexpected happened, or that a problem might occur
   in the near future. The software is still working as expected.
3. `info`: Confirmation that things are working as expected.
4. `debug`: Detailed information, typically only of interest to a developer trying to
   diagnose a problem.

## Usage

Courierr has two audiences:

1. Library developers that want to give their clients flexibility in message logging and error handling, and
2. Clients adopting the libraries developed by #1

### Library Developers

Developers add a `Courierr` (though we recommend using a `std::shared_ptr<Courierr::Courierr>`) data member to any
public class in a library where an error (or other loggable event) might occur.

```c++
#include <courierr/courierr.h>

class LibraryClass {
    public:
        LibraryClass(const int my_number_in, const std::string name_in, const std::shared_ptr<Courierr::Courierr>& courier_in) : 
            my_number(my_number_in), name(std::move(name_in)), 
    private:
        std::string name;
        int my_number;
        std::shared_ptr<Courierr::Courierr> courier;
};
```

The `Courierr` base class defines an interface with a public methods for each of the four message levels:

- `debug(const std::string& message)`
- `info(const std::string& message)`
- `warning(const std::string& message)`
- `error(const std::string& message)`

These methods should be called from the code within the library class.

```c++
courier->error("Something terrible happened.");
```

#### Good Practices

- The parent class should provide access to set and get the `Courierr` pointer.
- By making shared pointers and providing access, multiple closely-related objects can share a single `Courierr`
  instance.

### Library Adopters

Adopters must derive a class from `Courierr::Courierr`. The Courierr base class establishes four virtual functions:

- `debug_override(const std::string& message)`
- `info_override(const std::string& message)`
- `warning_override(const std::string& message)`
- `error_override(const std::string& message)`

#### Derived Class Recommendations

##### Common Message Format

##### Adding client context to messages

##### Add a log level