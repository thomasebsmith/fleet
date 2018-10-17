#include <string>
#include "Token.hpp"

Token::Token(): value(""), type(Token::Type::Comment) {}
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

bool Token::isOpeningGrouper() const {
  if (getType() != Type::Grouper) {
    return false;
  }
  std::string value = getValue();
  return value == "(" || value == "[" || value == "{";
}

// Note that this function returns a copy of the original token if it is not an
// opening grouper.
Token Token::matchingGrouper() const {
  std::string opener = getValue();
  Type type = getType();

  if (type != Type::Grouper) {
    return Token { opener, type };
  }

  std::string matching;
  if (opener == "(") {
    matching = ")";
  }
  else if (opener == "[") {
    matching = "]";
  }
  else if (opener == "{") {
    matching = "}";
  }
  else {
    return Token { opener, type };
  }
  return Token { matching, type };
}

bool Token::operator==(const Token &rhs) const {
  return getType() == rhs.getType() && getValue() == rhs.getValue();
}
bool Token::operator!=(const Token &rhs) const {
  return getType() != rhs.getType() || getValue() != rhs.getValue();
}
Token::operator std::string() const {
  Token::Type t = getType();
  std::string typeString;
  switch (t) {
    case Token::Type::Comment:
      typeString = "Comment";
      break;
    case Token::Type::Grouper:
      typeString = "Grouper";
      break;
    case Token::Type::Identifier:
      typeString = "Identifier";
      break;
    case Token::Type::LineBreak:
      typeString = "LineBreak";
      break;
    case Token::Type::Number:
      typeString = "Number";
      break;
    case Token::Type::Operator:
      typeString = "Operator";
      break;
    case Token::Type::String:
      typeString = "String";
      break;
  }
  return std::string("(" + typeString + ": " + getValue());
}
