#ifndef TOKENTREE_HPP
#define TOKENTREE_HPP

#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>
#include "Token.hpp"
#include "TokenStream.hpp"
#include "TokenTreeVisitor.hpp"

class TokenTree {
public:

  typedef std::shared_ptr<TokenTree> TreePointer;
  typedef std::pair<TreePointer, TreePointer> FunctionPair;
  typedef std::vector<TreePointer> LineList;

private:
  
  static std::unordered_map<std::string, int> precedences;
  static int defaultPrecedence;

  static std::unordered_map<std::string, bool> associativities;
  static bool defaultAssociativity;

  const std::variant<Token, FunctionPair, LineList> data;

public:

  TokenTree(const Token &value);
  TokenTree(const TreePointer &f, const TreePointer &x);
  TokenTree(const LineList &lines);
  TokenTree(const TokenTree &copyFrom);

  void accept(const TokenTreeVisitor &v);
  std::optional<Token> getToken();
  std::optional<std::pair<TokenTree, TokenTree>> getFunctionPair();
  std::optional<std::vector<TokenTree>> getLineList();

  static int getPrecedence(std::string op);
  static bool getAssociativity(std::string op);
  static TokenTree build(TokenStream stream);
};

#endif
