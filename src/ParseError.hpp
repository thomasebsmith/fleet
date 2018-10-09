#ifndef PARSEERROR_HPP
#define PARSEERROR_HPP

#include <string>

class ParseError: public std::runtime_error {
public:
  ParseError(std::string message);
  virtual const char *what() const throw();
};

#endif
