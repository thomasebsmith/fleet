#include <stdexcept>
#include <variant>
#include <vector>
#include "Context.hpp"
#include "Evaluator.hpp"
#include "FunctionValue.hpp"
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
  const Value::OrError &fValueOrErr = f.accept(*this);
  if (std::holds_alternative<std::runtime_error>(fValueOrErr)) {
    return fValueOrErr;
  }
  const auto &fValue = *std::get_if<Value::Pointer>(&fValueOrErr);

  if (x.isImplied()) {
    const auto &maybeReversible = std::dynamic_pointer_cast<
      ReversibleCallValue
    >(fValue);
    if (maybeReversible) {
      const auto &maybeReversed = maybeReversible->getReverse();
      if (std::holds_alternative<std::runtime_error>(maybeReversed)) {
        return maybeReversed;
      }
      const auto &reversed = *std::get_if<Value::Pointer>(&maybeReversed);
      return reversed;
    }
    return { TypeError {
      std::string { "Cannot reverse value of type " } + fValue->getName()
    } };
  }

  const Value::OrError &xValueOrErr = x.accept(*this);
  if (std::holds_alternative<std::runtime_error>(xValueOrErr)) {
    return xValueOrErr;
  }
  const auto &xValue = *std::get_if<Value::Pointer>(&xValueOrErr);

  return fValue->call(xValue);
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

Value::OrError Evaluator::visit() const {
  return { ParseError { "Internal error: Invalid implied argument in tree" } };
}
