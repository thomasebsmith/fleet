#include <string>
#include "TokenTree.hpp"
#include "FunctionValue.hpp"

FunctionValue::FunctionValue(
  const TokenTree &ast, const Context::Pointer &context, std::string param
): action { ast }, evaluator { context }, isNative { false },
  paramName { param } {}

FunctionValue::FunctionValue(
  const FunctionValue::NativeAction &func, const Context::Pointer &context
): action { func }, evaluator { context }, isNative { true }, paramName {""} {}

Value::OrError FunctionValue::call(Value::Pointer arg) const {
  if (std::holds_alternative<FunctionValue::NativeAction>(action)) {
    if (!isNative) {
      return { std::runtime_error { "Internal error: Invalid function" } };
    }
    return (*std::get_if<FunctionValue::NativeAction>(&action))(arg);
  }
  evaluator.tempDefine(paramName, arg);
  return evaluator.evaluate(*std::get_if<TokenTree>(&action));
}

bool FunctionValue::getIsNative() const {
  return isNative;
}

FunctionValue::operator std::string() const {
  if (getIsNative()) {
    return std::string { "<Native Function>" };
  }
  return std::string { "<Function>" };
}
