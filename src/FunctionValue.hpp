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

class ReversibleCallValue: public Value {
public:
  virtual Value::OrError getReverse() const = 0;
};

template <typename P, typename R>
class FunctionValueBase: public ReversibleCallValue {
public:
  typedef std::shared_ptr<R> ReturnPointer;
  typedef std::variant<std::runtime_error, ReturnPointer> Return;
  typedef std::function<Return(std::shared_ptr<P>)> NativeAction;
  typedef std::variant<TokenTree, NativeAction> Action;
private:
  const Action action;
  mutable Evaluator evaluator; // To allow temporary defining of parameters
  bool isNative;
  std::string paramName;
public:
  FunctionValueBase(
    const NativeAction &func, const Context::Pointer &context,
    bool makeNative = true
  ): action { func }, evaluator { context }, isNative { makeNative },
    paramName {""} {}
  
  Value::OrError getReverse() const {
    return {
      TypeError {
        std::string { "Cannot reverse function of type " } + getName()
      }
    };
  }

  Value::OrError call(Value::Pointer arg) const {
    const auto &castArg = arg->castValue<P>();
    if (!castArg) {
      return { TypeError {
        std::string { "Expected argument of type " } + P::name +
          " but got argument of type " + arg->getName()
      } };
    }
    const auto &castArgPtr = std::dynamic_pointer_cast<P>(arg);
    if (std::holds_alternative<NativeAction>(action)) {
      const auto &returnVal = (*std::get_if<NativeAction>(&action))(castArgPtr);
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
  FunctionValueBase(
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

  ~FunctionValueBase() = default;
};

template <typename P, typename R>
class FunctionValue: public FunctionValueBase<P, R> {
  // Should inherit everything from FunctionValueBase
  using FunctionValueBase<P, R>::FunctionValueBase;
};

template <typename P1, typename P2, typename RFinal>
class FunctionValue<P1, FunctionValue<P2, RFinal>>: public FunctionValueBase<
  P1, FunctionValue<P2, RFinal>
> {
private:
  typedef std::variant<std::runtime_error, std::shared_ptr<RFinal>> FinalReturn;
  typedef std::variant<std::runtime_error, std::shared_ptr<FunctionValue<
    P1, RFinal
  >>> FirstReturn;
public:
  using FunctionValueBase<P1, FunctionValue<P2, RFinal>>::FunctionValueBase;
  Value::OrError getReverse() const {
    return {
      Value::Pointer { new FunctionValue<P2, FunctionValue<P1, RFinal>> {
        [&] (const std::shared_ptr<P2> &secondParam) -> FirstReturn {
          return std::shared_ptr<FunctionValue<P1, RFinal>> {
          new FunctionValue<P1, RFinal> {
            [&, secondParam] (const std::shared_ptr<P1> &firstParam) ->
              FinalReturn {
              const auto &firstResultOrErr = this->call(firstParam);
              if (std::holds_alternative<std::runtime_error>(
                firstResultOrErr
              )) {
                return { *std::get_if<std::runtime_error>(&firstResultOrErr) };
              }
              const auto &firstResult = *std::get_if<Value::Pointer>(
                &firstResultOrErr
              );
              const auto &finalResultOrErr = firstResult->call(secondParam);
              if (std::holds_alternative<std::runtime_error>(
                finalResultOrErr
              )) {
                return { *std::get_if<std::runtime_error>(&finalResultOrErr) };
              }
              const auto &finalResult = *std::get_if<Value::Pointer>(
                &finalResultOrErr
              );
              const auto &finalResultCast = std::dynamic_pointer_cast<RFinal>(
                finalResult
              );

              if (!finalResultCast) {
                return FinalReturn { TypeError {
                  std::string { "Expected return value of type " } +
                  RFinal::name + " but got return value of type " +
                  finalResult->getName()
                } };
              }
              return FinalReturn { finalResultCast };
            },
            Context::Pointer { new Context {} },
            this->getIsNative()
          }
          };
        },
        Context::Pointer { new Context {} },
        this->getIsNative()
      } }
    };
  }
};

template <typename P, typename R>
const std::string FunctionValueBase<P, R>::name { P::name + "->" + R::name };

#endif
