#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include <unordered_map>
#include <vector>
#include "Context.hpp"
#include "Token.hpp"
#include "TokenTree.hpp"
#include "TokenTreeVisitor.hpp"
#include "Value.hpp"

class Evaluator: public TokenTreeVisitor<Value::OrError> {
private:
  Context::Pointer evaluationContext;
  bool removeContextLayer = false;
public:
  Evaluator(const Context::Pointer &context);
  Value::OrError evaluate(const TokenTree &ast);
  void tempDefine(const std::string &name, Value::Pointer value);
  Value::OrError visit(const Token &token) const;
  Value::OrError visit(const TokenTree &f, const TokenTree &x) const;
  Value::OrError visit(const std::vector<TokenTree> &lines) const;
};

#endif
