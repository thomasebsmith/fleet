// File: TypeError.hpp
// Purpose: Header file for TypeErrors, which are a type of runtime_error
//  occurring when code mismatches types.

#ifndef TYPEERROR_HPP
#define TYPEERROR_HPP

#include <string>

class TypeError: public std::runtime_error {
public:
  // Constructor(message) - Creates a TypeError with the given internal message.
  TypeError(std::string message);

  // what() - Returns the internal message of the TypeError, preceded by
  //  "TypeError: ".
  virtual const char *what() const throw();
};

#endif
