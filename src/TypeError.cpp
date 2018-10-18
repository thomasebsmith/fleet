#include <stdexcept>
#include <string>
#include "TypeError.hpp"

TypeError::TypeError(std::string message): runtime_error(message) {}
const char *TypeError::what() const throw() {
  return (std::string("TypeError: ") + runtime_error::what()).c_str();
}
