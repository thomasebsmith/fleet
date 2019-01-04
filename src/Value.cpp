// File: src/Value.hpp
// Purpose: Source file for an abstract class from which all Fleet Values should
//  inherit. Examples of Values include NumberValues, FunctionValues, etc.

#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <variant>
#include "Value.hpp"
#include "Evaluator.hpp"
#include "TokenTree.hpp"

Value::OrError Value::call(const TokenTree &ast, const Evaluator *eval) const {
  // If the argument is not implied, evaluate the argument TokenTree.
  const Value::OrError &xValueOrErr = ast.accept(*eval);
  if (std::holds_alternative<std::runtime_error>(xValueOrErr)) {
    return xValueOrErr;
  }
  const auto &xValue = *std::get_if<Value::Pointer>(&xValueOrErr);

  return call(xValue);
}

const std::string Value::name { "Value" };

std::string Value::getClassName() {
  return Value::name;
}
