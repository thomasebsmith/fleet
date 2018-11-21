// File: src/ParseError.cpp
// Purpose: A ParseError is a type of runtime_error that occurs during the
//  parsing of code (i.e. during tokenization or token tree formation). For
//  more documentation, see src/ParseError.hpp.

#include <stdexcept>
#include <string>
#include "ParseError.hpp"

// Constructor
ParseError::ParseError(std::string message): runtime_error(message) {}

// what() - Returns the content of the error message, preceded by "ParseError: "
const char *ParseError::what() const throw() {
  return (std::string("ParseError: ") + runtime_error::what()).c_str();
}
