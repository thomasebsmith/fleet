// File: src/Token.cpp
// Purpose: A Token is a single "word" in a programming language. It could be
//  an identifier, an operator, a parenthesis or bracket, a number, a string,
//  a comment, or a line break. For more documentation, see src/Token.hpp.

#include <string>
#include "Token.hpp"

// Constructors
Token::Token(): value(""), type(Token::Type::Comment) {}
Token::Token(std::string value, Token::Type type): value(value), type(type) {}

// getType() - Returns the type of the Token.
Token::Type Token::getType() const {
  return type;
}

// getValue() - Returns the value of the Token.
std::string Token::getValue() const {
  return value;
}

// setType(newType) - Sets the Token's type to newType.
void Token::setType(Token::Type newType) {
  type = newType;
}

// setValue(newValue) - Sets the Token's value to newValue.
void Token::setValue(std::string newValue) {
  value = newValue;
}

// isOpeningGrouper() - Returns a boolean indicating whether the Token is a
//  an opening grouper (i.e. [, (, or { ).
bool Token::isOpeningGrouper() const {
  if (getType() != Type::Grouper) {
    return false;
  }
  std::string value = getValue();
  return value == "(" || value == "[" || value == "{";
}

// matchingGrouper() - Returns the matching grouper for a given Token, or a copy
//  of the original Token if the Token is not a grouper.
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

// ==rhs - Returns true iff the Token's value and type are the same as rhs's
//  value and type.
bool Token::operator==(const Token &rhs) const {
  return getType() == rhs.getType() && getValue() == rhs.getValue();
}

// !=rhs - Returns the opposite of ==rhs.
bool Token::operator!=(const Token &rhs) const {
  return getType() != rhs.getType() || getValue() != rhs.getValue();
}

// operator string() - Returns a string representation of the Token in the form
//  (type: value).
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
