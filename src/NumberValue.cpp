#include <stdexcept>
#include <variant>
#include "NumberValue.hpp"
#include "TypeError.hpp"
#include "Value.hpp"

NumberValue::NumberValue(double rawNumber): number(rawNumber) {}
NumberValue::NumberValue(const NumberValue &other): number(other.rawNumber) {}
NumberValue::NumberValue(const Token &numberToken):
  number(stod(numberToken.getValue())) {}
std::variant<std::runtime_error> call(Value::Pointer arg) const {
  return { { "Value of type Number cannot be called" } };
}
double NumberValue::getRawNumber() const {
  return number;
}
