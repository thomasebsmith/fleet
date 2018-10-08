#ifndef TOKEN_HPP
#define TOKEN_HPP

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
  Token(std::string tokenValue, Type tokenType);
  Type getType() const;
  std::string getValue() const;
  void setType(Type newType);
  void setValue(std::string newValue);
};

#endif
