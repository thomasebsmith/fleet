#ifndef TOKENTREE_HPP
#define TOKENTREE_HPP

#include <string>
#include <utility>
#include <vector>
#include "ParseError.hpp"
#include "Token.hpp"
#include "TokenStream.hpp"

class TokenTree {
private:
  static std::unordered_map<std::string, int> precedences;
  static int defaultPrecedence;

  static std::unordered_map<std::string, bool> associativities;
  static bool defaultAssociativity;

public:
  const union {
    Token endpoint;
    std::pair<
      std::shared_ptr<TokenTree>,
      std::shared_ptr<TokenTree>
    > functionCall;
    std::vector<std::shared_ptr<TokenTree>> multiLine;
  };
  enum class Type {
    Endpoint,
    FunctionCall,
    MultiLine
  };
  const Type type;

  TokenTree(Token value);
  ~TokenTree();
  TokenTree(std::shared_ptr<TokenTree> f, std::shared_ptr<TokenTree> x);
  TokenTree(std::vector<std::shared_ptr<TokenTree>> lines);
  
  static int getPrecedence(std::string op);
  static bool getAssociativity(std::string op);
  static TokenTree build(TokenStream stream);
};

#endif
