#include <stdexcept>
#include <string>
#include "ParseError.hpp"

ParseError::ParseError(std::string message): runtime_error(message) {}
const char *ParseError::what() const throw() {
  return runtime_error::what();
}
