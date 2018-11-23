// File: src/Token.hpp
// Purpose: A Token is a single "word" in a programming language. It could be
//  an identifier, an operator, a parenthesis or bracket, a number, a string,
//  a comment, or a line break. Each Token also contains a value - the string
//  that formed the Token. For implementations, see src/Token.cpp.

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

class Token {
public:
  enum class Type {
    Comment,
    Grouper,
    Identifier,
    LineBreak,
    Number,
    Operator,
    String
  };
private:
  std::string value;
  Type type;

public:
  // Constructor() - Creates a Token containing an empty comment.
  Token();
  
  // Constructor(tokenValue, tokenType) - Creates a Token with the given
  //  value and type.
  Token(std::string tokenValue, Type tokenType);

  // getType() - Returns the Token's type.
  Type getType() const;

  // getValue() - Returns the Token's value.
  std::string getValue() const;

  // setType(newType) - Sets the Token's type to newType.
  void setType(Type newType);

  // setValue(newValue) - Sets the Token's value to newValue.
  void setValue(std::string newValue);

  // isOpeningGrouepr() - Returns a boolean that is true if and only if the
  //  Token is a grouper and contains "(", "[", or "{".
  bool isOpeningGrouper() const;

  // matchingGrouper() - Returns the matching grouper for a given grouper Token
  //  (e.g. "(" for ")" or "}" for "{") or a copy of the Token if the Token
  //  is not a grouper.
  Token matchingGrouper() const;

  // ==rhs - Returns a boolean that is true if and only if the Token's type
  //  and value are equal to rhs's type and value.
  bool operator==(const Token &rhs) const;

  // !=rhs - Returns a boolean that is true if and only if the Token's type
  //  or value is not equal to rhs's type or value.
  bool operator!=(const Token &rhs) const;

  // operator string() - Returns a string representation of the Token in the
  //  form "(type: value)".
  operator std::string() const;
};

#endif
