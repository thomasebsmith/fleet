#include <stdexcept>
#include <variant>
#include <vector>
#include "Context.hpp"
#include "Evaluator.hpp"
#include "NumberValue.hpp"
#include "ParseError.hpp"
#include "Token.hpp"
#include "TokenTree.hpp"
#include "Value.hpp"

Evaluator::Evaluator(const Context::Pointer &context):
  evaluationContext { context } {}

Value::OrError Evaluator::evaluate(const TokenTree &ast) {
  bool wasRemoveContextLayer = removeContextLayer;
  removeContextLayer = false;
  const Value::OrError &result = ast.accept(*this);
  if (wasRemoveContextLayer) {
    evaluationContext =
      Context::Pointer { evaluationContext->getParentContext() };
  }
  return result;
}

void Evaluator::tempDefine(const std::string &name, Value::Pointer value) {
  Context::Pointer newContext { new Context { evaluationContext } };
  if (newContext->define(name, value)) {
    throw ParseError { "Internal error: Could not define in function call" };
  }
  evaluationContext = Context::Pointer { newContext };
  removeContextLayer = true;
}

Value::OrError Evaluator::visit(const Token &token) const {
  switch (token.getType()) {
    case Token::Type::Identifier:
    case Token::Type::Operator:
      return evaluationContext->getValue(token.getValue());
    case Token::Type::Number:
      return { Value::Pointer { 
        new NumberValue { std::stod(token.getValue()) }
      } };
    // TODO: String
    default:
      return { ParseError { "Internal error: Invalid token type in tree" } };
  }
}

Value::OrError Evaluator::visit(const TokenTree &f, const TokenTree &x) const {
  const Value::OrError &fValue = f.accept(*this);
  if (std::holds_alternative<std::runtime_error>(fValue)) {
    return fValue;
  }
  const Value::OrError &xValue = x.accept(*this);
  if (std::holds_alternative<std::runtime_error>(xValue)) {
    return xValue;
  }
  return (*std::get_if<Value::Pointer>(&fValue))->call(
    *std::get_if<Value::Pointer>(&xValue)
  );
}

Value::OrError Evaluator::visit(const std::vector<TokenTree> &lines) const {
  // TODO: New Context?
  Value::OrError lastValue { ParseError { "Invalid empty code block " } };
  for (size_t i = 0; i < lines.size(); i++) {
    lastValue = lines[i].accept(*this);
    if (std::holds_alternative<std::runtime_error>(lastValue)) {
      return lastValue;
    }
  }
  return lastValue;
}
