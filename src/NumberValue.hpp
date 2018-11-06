#ifndef NUMBERVALUE_HPP
#define NUMBERVALUE_HPP

#include <string>
#include "Token.hpp"
#include "Value.hpp"

class NumberValue: public Value {
private:
  double number;
public:
  NumberValue(double rawNumber);
  NumberValue(const NumberValue &other);
  NumberValue(const Token &numberToken);
  ~NumberValue() = default;
  Value::OrError call([[maybe_unused]] Value::Pointer arg) const;
  operator std::string() const;
  double getRawNumber() const;
  static const std::string name;
  std::string getName() const;
};

#endif
