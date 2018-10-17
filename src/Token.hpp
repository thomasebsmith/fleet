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
  Token();
  Token(std::string tokenValue, Type tokenType);
  Type getType() const;
  std::string getValue() const;
  void setType(Type newType);
  void setValue(std::string newValue);
  bool isOpeningGrouper() const;
  Token matchingGrouper() const;
  bool operator==(const Token &rhs) const;
  bool operator!=(const Token &rhs) const;
  operator std::string() const;
};

#endif
