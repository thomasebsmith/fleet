#ifndef TYPEERROR_HPP
#define TYPEERROR_HPP

#include <string>

class TypeError: public std::runtime_error {
public:
  TypeError(std::string message);
  virtual const char *what() const throw();
};

#endif
