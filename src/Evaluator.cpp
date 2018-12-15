// File: src/Evaluator.cpp
// Purpose: Source file for Evaluators, which are tied to a Context and obtain
// the result of evaluating some Fleet in that Context. See src/Evaluator.hpp
// for more documentation.

#include <stdexcept>
#include <variant>
#include <vector>
#include "Evaluator.hpp"
#include "Context.hpp"
#include "FunctionValue.hpp"
#include "NumberValue.hpp"
#include "ParseError.hpp"
#include "Token.hpp"
#include "TokenTree.hpp"
#include "Value.hpp"

// This constructor creates an Evaluator with the given Context.
Evaluator::Evaluator(const Context::Pointer &context):
  evaluationContext { context } {}

// This method returns the result of evaluating the given TokenTree in the
// evaluator's Context. It uses the fact that each Evaluator is a
// TokenTreeVisitor (i.e. see the Evaluator::visit methods for further
// implementation).
Value::OrError Evaluator::evaluate(const TokenTree &ast) {
  bool wasRemoveContextLayer = removeContextLayer;
  removeContextLayer = false;

  // Evaluate the tree using this class as a TokenTreeVisitor.
  const Value::OrError &result = ast.accept(*this);

  // Remove the current Context and go to the parent Context if necessary.
  // This should only be necessary if a variable was temporarily defined
  // using the tempDefine method.
  if (wasRemoveContextLayer) {
    evaluationContext =
      Context::Pointer { evaluationContext->getParentContext() };
  }
  return result;
}

// This method defines a variable as a value for the next evaluation (i.e.
// the next call to the evaluate method).
void Evaluator::tempDefine(const std::string &name, Value::Pointer value) {
  // Create a new Context with the current Context as the parent Context.
  Context::Pointer newContext { new Context { evaluationContext } };

  // If there is an error, this function throws an error.
  if (newContext->define(name, value)) {
    throw ParseError { "Internal error: Could not define in function call" };
  }
  evaluationContext = Context::Pointer { newContext };
  removeContextLayer = true;
}

// This method accepts a Token and converts it into a Value Pointer or an error
// if it cannot be converted. Note that strings conversions are not yet
// implemented.
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

// This method accepts two TokenTrees (for a function and an argument) and
// returns the result of calling the function with the argument if possible.
Value::OrError Evaluator::visit(const TokenTree &f, const TokenTree &x) const {
  // Evaluate the function TokenTree.
  const Value::OrError &fValueOrErr = f.accept(*this);
  if (std::holds_alternative<std::runtime_error>(fValueOrErr)) {
    return fValueOrErr;
  }
  const auto &fValue = *std::get_if<Value::Pointer>(&fValueOrErr);

  // If the argument is implied (i.e. there is no first argument), return the
  // reverse of the function. An implied function tree results from syntax
  // like (+ 3), which returns the reverse of the (+) function with 3 as its
  // first argument.
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

  // If the argument is not implied, evaluate the argument TokenTree.
  const Value::OrError &xValueOrErr = x.accept(*this);
  if (std::holds_alternative<std::runtime_error>(xValueOrErr)) {
    return xValueOrErr;
  }
  const auto &xValue = *std::get_if<Value::Pointer>(&xValueOrErr);

  // Return the result of calling the function with the argument.
  return fValue->call(xValue);
}

// This method converts a vector of TokenTrees into a Value Pointer. It
// evaluates each line and returns the result of the last line if there is
// no error. If there is an error, it returns the first error without
// evaluating further lines.
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

// This method is used for visiting implied arguments. This should not occur,
// since implied arguments should be detected when they are function
// arguments (see visit(f, x)).
Value::OrError Evaluator::visit() const {
  return { ParseError { "Internal error: Invalid implied argument in tree" } };
}
