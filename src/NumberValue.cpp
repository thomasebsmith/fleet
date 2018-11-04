#include <stdexcept>
#include <string>
#include "NumberValue.hpp"
#include "TypeError.hpp"
#include "Value.hpp"

NumberValue::NumberValue(double rawNumber): number(rawNumber) {}
NumberValue::NumberValue(const NumberValue &other): number(other.number) {}
NumberValue::NumberValue(const Token &numberToken):
  number(stod(numberToken.getValue())) {}
Value::OrError NumberValue::call([[maybe_unused]] Value::Pointer arg) const {
  return { std::runtime_error { "Value of type Number cannot be called" } };
}
NumberValue::operator std::string() const {
  return std::to_string(number);
}
double NumberValue::getRawNumber() const {
  return number;
}
