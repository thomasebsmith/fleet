// File: src/ParseError.hpp
// Purpose: A ParseError is a type of runtime_error that occurs during the
//  parsing of code (i.e. during tokenization or token tree formation). For
//  implementations see src/ParseError.cpp.

#ifndef PARSEERROR_HPP
#define PARSEERROR_HPP

#include <string>

class ParseError: public std::runtime_error {
public:
  // Constructor(message) - Creates a ParseError with the given message
  ParseError(std::string message);

  // what() - Returns the ParseError's message, preceded by the string
  //  "ParseError: ".
  virtual const char *what() const throw();
};

#endif
