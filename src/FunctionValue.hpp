#ifndef FUNCTIONVALUE_HPP
#define FUNCTIONVALUE_HPP

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>
#include "Context.hpp"
#include "Evaluator.hpp"
#include "TokenTree.hpp"
#include "TypeError.hpp"
#include "Value.hpp"

template <typename P, typename R>
class FunctionValue: public Value {
public:
  typedef std::shared_ptr<R> ReturnPointer;
  typedef std::variant<std::runtime_error, ReturnPointer> Return;
  typedef std::function<Return(P)> NativeAction;
  typedef std::variant<TokenTree, NativeAction> Action;
private:
  const Action action;
  mutable Evaluator evaluator; // To allow temporary defining of parameters
  bool isNative;
  std::string paramName;
public:
  FunctionValue(
    const NativeAction &func, const Context::Pointer &context,
    bool makeNative = true
  ): action { func }, evaluator { context }, isNative { makeNative },
    paramName {""} {}

  Value::OrError call(Value::Pointer arg) const {
    const auto &castArg = arg->castValue<P>();
    if (!castArg) {
      return { TypeError {
        std::string { "Expected argument of type " } + P::name +
          " but got argument of type " + arg->getName()
      } };
    }
    if (std::holds_alternative<NativeAction>(action)) {
      const auto &returnVal = (*std::get_if<NativeAction>(&action))(*castArg);
      if (std::holds_alternative<std::runtime_error>(returnVal)) {
        return { *std::get_if<std::runtime_error>(&returnVal) };
      }
      return { *std::get_if<ReturnPointer>(&returnVal) };
    }
    evaluator.tempDefine(paramName, arg);
    const auto &returnValOrErr = evaluator.evaluate(
      *std::get_if<TokenTree>(&action)
    );
    if (std::holds_alternative<std::runtime_error>(returnValOrErr)) {
      return returnValOrErr;
    }
    Value::Pointer returnVal = *std::get_if<Value::Pointer>(&returnValOrErr);
    const auto &castReturnVal = returnVal->castValue<R>();
    if (!castReturnVal) {
      return { TypeError {
        std::string { "Expected return value of type " } + R::name +
          " but got return value of type " + returnVal->getName()
      } };
    }
    return { returnVal };
  }

  static const std::string name;
  std::string getName() const {
      return name;
  }
  FunctionValue(
    const TokenTree &ast, const Context::Pointer &context, std::string param
  ): action { ast }, evaluator { context }, isNative { false },
    paramName { param } {}

  bool getIsNative() const {
    return isNative;
  }

  operator std::string() const {
    if (getIsNative()) {
      return std::string { "<Native " } + getName() + ">";
    }
    return std::string { "<Function " } + getName() + ">";
  }

  ~FunctionValue() = default;
};

template<typename P, typename R>
const std::string FunctionValue<P, R>::name { P::name + "->" + R::name };

#endif
