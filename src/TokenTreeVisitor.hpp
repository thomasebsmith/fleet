// File: src/TokenTreeVisitor.hpp
// Purpose: Header file for the abstract class TokenTreeVisitor, which
//  represents a class that can convert a TokenTree of any type into a value of
//  template type T.

#ifndef TOKENTREEVISITOR_HPP
#define TOKENTREEVISITOR_HPP

#include <vector>
#include "Token.hpp"
#include "TokenTree.hpp"

class TokenTree;

template <typename T>
class TokenTreeVisitor {
public:
  // virtual visit(token) - Visitor method for Token-only TokenTrees.
  virtual T visit(const Token &token) const = 0;

  // virtual visit(f, x) - Visitor method for function pair TokenTrees.
  virtual T visit(const TokenTree &f, const TokenTree &x) const = 0;

  // virtual visit(lines) - Visitor method for line list TokenTrees.
  virtual T visit(const std::vector<TokenTree> &lines) const = 0;

  // virtual visit() - Visitor method for implied argument TokenTrees.
  virtual T visit() const = 0;
};

#endif
