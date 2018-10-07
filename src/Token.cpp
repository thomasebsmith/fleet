#include <string>
#include "Token.hpp"

Token::Token(std::string value, Token::Type type): value(value), type(type) {}

Token::Type Token::getType() const {
  return type;
}
std::string Token::getValue() const {
  return value;
}
void Token::setType(Token::Type newType) {
  type = newType;
}
void Token::setValue(std::string newValue) {
  value = newValue;
}
