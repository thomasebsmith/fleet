// File: TypeError.cpp
// Purpose: Source file for TypeErrors, which are a type of runtime_error
//  occurring when code mismatches types.

#include <stdexcept>
#include <string>
#include "TypeError.hpp"

// Constructor
TypeError::TypeError(std::string message): runtime_error(message) {}

// what() - Returns the content of the error message, preceded by "TypeError: "
const char *TypeError::what() const throw() {
  return (std::string("TypeError: ") + runtime_error::what()).c_str();
}
