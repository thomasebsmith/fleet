// File: src/NumberValue.hpp
// Purpose: A NumberValue is a Value that contains a floating point number.
//  The actual C++ double inside the class should be accessed by built in
//  functions. Fleet code can build more functions from these built in
//  functions. For implementations see src/NumberValue.cpp.

#ifndef NUMBERVALUE_HPP
#define NUMBERVALUE_HPP

#include <string>
#include "Token.hpp"
#include "Value.hpp"

class NumberValue: public Value {
private:
  double number;
public:
  // Constructor(rawNumber) - Creates a NumberValue with rawNumber as its
  //  internal C++ double.
  NumberValue(double rawNumber);
  // Constructor(other) - Creates a copy of other.
  NumberValue(const NumberValue &other);
  // Constructor(numberToken) - Creates a NumberValue with its internal number
  //  as the value of the token.
  NumberValue(const Token &numberToken);

  // Destructor - Default
  ~NumberValue() = default;

  // call(arg) - Returns an error, since NumberValues cannot be called.
  Value::OrError call([[maybe_unused]] Value::Pointer arg) const;

  // operator string() - Returns the string reprentation of the NumberValue's
  //  internal number.
  operator std::string() const;

  // getRawNumber() - Returns the NumberValue's internal C++ double.
  double getRawNumber() const;

  // name/getName() - Returns "Number", the name of a NumberValue-type value.
  static const std::string name;
  static std::string getClassName();
  std::string getName() const;
};

#endif
