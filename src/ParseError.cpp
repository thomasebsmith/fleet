#include <stdexcept>
#include <string>
#include "ParseError.hpp"

ParseError::ParseError(std::string message): runtime_error(message) {}
const char *ParseError::what() const throw() {
  return (std::string("ParseError: ") + runtime_error::what()).c_str();
}
