#ifndef TOKENTREE_HPP
#define TOKENTREE_HPP

#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "ParseError.hpp"
#include "Token.hpp"
#include "TokenStream.hpp"

class TokenTree {
public:
  enum class Type {
    Endpoint,
    FunctionCall,
    MultiLine
  };

private:
  static std::unordered_map<std::string, int> precedences;
  static int defaultPrecedence;

  static std::unordered_map<std::string, bool> associativities;
  static bool defaultAssociativity;

  const union {
    Token endpoint;
    std::pair<
      std::shared_ptr<TokenTree>,
      std::shared_ptr<TokenTree>
    > functionCall;
    std::vector<std::shared_ptr<TokenTree>> multiLine;
  };
  const Type type;

public:
  TokenTree(Token value);
  ~TokenTree();
  TokenTree(std::shared_ptr<TokenTree> f, std::shared_ptr<TokenTree> x);
  TokenTree(std::vector<std::shared_ptr<TokenTree>> lines);
  TokenTree(const TokenTree &copyFrom);

  Type getType();
  std::optional<Token> getEndpoint();
  std::optional<std::pair<TokenTree, TokenTree>> getFunctionCall();
  std::optional<std::vector<TokenTree>> getMultiLine();
  
  static int getPrecedence(std::string op);
  static bool getAssociativity(std::string op);
  static TokenTree build(TokenStream stream);
};

#endif
