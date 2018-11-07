#ifndef TOKENTREEVISITOR_HPP
#define TOKENTREEVISITOR_HPP

#include <vector>
#include "Token.hpp"
#include "TokenTree.hpp"

class TokenTree;

template <typename T>
class TokenTreeVisitor {
public:
  virtual T visit(const Token &token) const = 0;
  virtual T visit(const TokenTree &f, const TokenTree &x) const = 0;
  virtual T visit(const std::vector<TokenTree> &lines) const = 0;
  virtual T visit() const = 0;
};

#endif
