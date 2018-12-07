// File: src/TokenStream.hpp
// Purpose: A TokenStream converts a string into a sequence of Tokens (i.e.
//  the smallest units of Fleet syntax). For implementations, see
//  src/TokenStream.cpp

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

  // Private methods are documented in src/TokenStream.cpp.
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
  // Constructor(code) - Creates a TokenStream with the code string to be parsed
  //  internal as `code`.
  TokenStream(std::string code);

  // peek() - Returns the next Token (that has not been previously retrieved by
  //  next()).
  Token peek();

  // next() - Returns the next Token (that has not been previously retrieved by
  //  this method).
  Token next();

  // hasNext() - Returns a boolean indicating whether the entire code string has
  //  been parsed and there are no Tokens remaining. Returns true iff there are
  //  more Tokens to be retrieved via next() or peek().
  bool hasNext();
};

#endif
