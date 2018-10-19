#ifndef NUMBERVALUE_HPP
#define NUMBERVALUE_HPP

#include <stdexcept>
#include <variant>
#include "Token.hpp"
#include "Value.hpp"

class NumberValue: public Value {
private:
  double number;
public:
  NumberValue(double rawNumber);
  NumberValue(const NumberValue &other);
  NumberValue(const Token &numberToken);
  std::variant<std::runtime_error, Value::Pointer> call(
    Value::Pointer arg
  ) const;
  double getRawNumber() const;
}

#endif
