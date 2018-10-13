#ifndef TOKENTREEVISITOR_HPP
#define TOKENTREEVISITOR_HPP

#include <vector>
#include "Token.hpp"
#include "TokenTree.hpp"

class TokenTree;

class TokenTreeVisitor {
public:
  virtual void visit(const Token &token) const = 0;
  virtual void visit(const TokenTree &f, const TokenTree &x) const = 0;
  virtual void visit(const std::vector<TokenTree> &lines) const = 0;
};

#endif
