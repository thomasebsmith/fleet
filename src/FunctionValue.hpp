#ifndef FUNCTIONVALUE_HPP
#define FUNCTIONVALUE_HPP

#include <functional>
#include <string>
#include <variant>
#include "Context.hpp"
#include "Evaluator.hpp"
#include "TokenTree.hpp"
#include "Value.hpp"

class FunctionValue: public Value {
public:
  typedef std::function<Value::OrError(Value::Pointer)> NativeAction;
  typedef std::variant<TokenTree, NativeAction> Action;
private:
  const Action action;
  mutable Evaluator evaluator; // To allow temporary defining of parameters
  bool isNative;
  std::string paramName;
  
public:
  FunctionValue(
    const TokenTree &ast, const Context::Pointer &context, std::string param
  );
  FunctionValue(const NativeAction &func, const Context::Pointer &context);
  ~FunctionValue() = default;
  Value::OrError call(Value::Pointer arg) const;
  bool getIsNative() const;
  operator std::string() const;
};

#endif
