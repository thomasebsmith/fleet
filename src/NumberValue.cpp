// File: src/NumberValue.cpp
// Purpose: A NumberValue is a Value that contains a floating point number.
//  The actual C++ double inside the class should be accessed by built in
//  functions. Fleet code can build more functions from these built in
//  functions. For more documentation see src/NumberValue.hpp.

#include <stdexcept>
#include <string>
#include "NumberValue.hpp"
#include "TypeError.hpp"
#include "Value.hpp"

// Constructors
NumberValue::NumberValue(double rawNumber): number(rawNumber) {}
NumberValue::NumberValue(const NumberValue &other): number(other.number) {}
NumberValue::NumberValue(const Token &numberToken):
  number(stod(numberToken.getValue())) {}

// call([unused] arg) - Returns an error, since NumberValues cannot be called.
Value::OrError NumberValue::call([[maybe_unused]] Value::Pointer arg) const {
  return { std::runtime_error { std::string {
    "Value of type "
  } + name + " cannot be called" } };
}

// operator string() - Returns the string representation of the internally
//  stored double.
NumberValue::operator std::string() const {
  return std::to_string(number);
}

// getRawNumber() - Returns the internal double value in the object.
double NumberValue::getRawNumber() const {
  return number;
}

const std::string NumberValue::name { "Number" };

// getName() - Returns "Number", the name of the Number type.
std::string NumberValue::getName() const {
  return NumberValue::name;
}

std::string NumberValue::getClassName() {
  return NumberValue::name;
}
