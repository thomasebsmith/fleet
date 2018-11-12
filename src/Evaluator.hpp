// File: src/Evaluator.cpp
// Purpose: Header file for Evaluators, which are tied to a Context and obtain
// the result of evaluating some Fleet in that Context. See src/Evaluator.cpp
// for implementations.

#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include <unordered_map>
#include <vector>
#include "Context.hpp"
#include "Token.hpp"
#include "TokenTree.hpp"
#include "TokenTreeVisitor.hpp"
#include "Value.hpp"

// The Evaluator class is a TokenTreeVisitor returning a type of Value::OrError.
//  This means that it can visit a TokenTree (using tree.visit(*this)) and
//  will always return a Value::OrError no matter the TokenTree. The accepting/
//  visting paradigm should be used only internally. Use the evaluate(ast)
//  method for code evaluation.
class Evaluator: public TokenTreeVisitor<Value::OrError> {
private:
  Context::Pointer evaluationContext;
  bool removeContextLayer = false;
public:
  // Constructor(context) - Creates an Evaluator with the given Context as its
  //  evaluation Context (i.e. the context in which it will parse code).
  Evaluator(const Context::Pointer &context);

  // evaluate(ast) - Evaluates the given TokenTree and returns either a Value
  //  Pointer or an error depending on the result of the code.
  Value::OrError evaluate(const TokenTree &ast);

  // tempDefine(name, value) - Temporarily (for only the next call of
  //  evaluate(ast)) defines a variable with the given name and value. Note
  //  that this creates a temporary child context rather than adding it to
  //  the evaluation context directly.
  void tempDefine(const std::string &name, Value::Pointer value);

  // visit(token) - Returns the result of converting the given Token to a Value.
  Value::OrError visit(const Token &token) const;

  // visit(f, x) - Returns the result of evaluating the given function call.
  Value::OrError visit(const TokenTree &f, const TokenTree &x) const;

  // visit(lines) - Returns the result of evaluating the given set of lines of
  //  code. Note that the value of the last line is returned if there are no
  //  errors, or the first error is returned if there are errors.
  Value::OrError visit(const std::vector<TokenTree> &lines) const;

  // visit() - Returns an error, since this method is used for visiting implied
  //  arguments, which should only exist within function calls and should be
  //  parsed within function calls. This method should never be called.
  Value::OrError visit() const;
};

#endif
