#ifndef TOKENSTREAM_HPP
#define TOKENSTREAM_HPP

#include <optional>
#include <string>
#include "ParseError.hpp"
#include "Token.hpp"

class TokenStream {
private:
  const std::string code;
  size_t index {0};
  std::optional<Token> nextToken {};
  std::optional<Token> lastToken {};
  void queueNext();
  
  static bool isblank(char c);

  std::string takeWhitespace();
  std::string takeComment();
  std::string takeGrouper();
  std::string takeIdentifier();
  std::string takeLineBreak();
  std::string takeNumber();
  std::string takeOperator();
  std::string takeString();
  std::string takeEscape();
public:
  TokenStream(std::string code);
  Token peek();
  Token next();
  bool hasNext();
};

#endif
