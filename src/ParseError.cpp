#include <stdexcept>

ParseError::ParseError(std::string message): runtime_error(message) {}
virtual const char *ParseError::what() const throw() {
  return runtime_error::what();
}
