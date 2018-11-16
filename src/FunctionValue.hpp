// File: src/FunctionValue.hpp
// Purpose: Header file for FunctionValues, which are a template class used to
//  contain a VAlue that can be called with a certain type to produce a
//  certain type. This file also contains the ReversibleCallValue abstract
//  class, which should be subclassed for any Values that can be called in
//  reverse.

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

// ReversibleCallValue - Represents any Value that can be reversed when called.
//  Normally, this applies to functions when they are called (operator param).
class ReversibleCallValue: public Value {
public:
  // getReverse() - This method must be created by subclasses. It should return
  //  a version of the Value that, when called with two parameters, produces
  //  a Value equivalent to when the original Value was called with the
  //  parameters in the reverse order.
  virtual Value::OrError getReverse() const = 0;
};

// FunctionValueBase<P, R> - Should not be used directly. All FunctionValues
//  inherit from this class; this class is simply used to reduce code 
//  repetition. However, the public methods in this class can be used on all
//  FunctionValues.
template <typename P, typename R>
class FunctionValueBase: public ReversibleCallValue {
public:
  // These typedefs can be used in place of their respective types.
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
  // Constructor(func, context, makeNative) - Creates a FunctionValueBase with
  //  a native function as its action. makeNative (which defaults to true)
  //  determines whether the function appears to be native within the code.
  FunctionValueBase(
    const NativeAction &func, const Context::Pointer &context,
    bool makeNative = true
  ): action { func }, evaluator { context }, isNative { makeNative },
    paramName {""} {}
  
  // getReverse() - Functions cannot be reversed unless they return functions.
  //  A specific subclass is used for this case, so by default, functions cannot
  //  be reversed.
  Value::OrError getReverse() const {
    return {
      TypeError {
        std::string { "Cannot reverse function of type " } + getName()
      }
    };
  }

  // call(arg) - Calls the function with the given argument. This will return
  //  an error if the type is incorrect, or it will return the result of the
  //  internal function action.
  Value::OrError call(Value::Pointer arg) const {
    // Attempt to cast arg to the appropriate parameter type. If it cannot be
    //  cast, return an error.
    const auto &castArg = arg->castValue<P>();
    if (!castArg) {
      return { TypeError {
        std::string { "Expected argument of type " } + P::name +
          " but got argument of type " + arg->getName()
      } };
    }

    // If the internal action is a native C++ function, call it with a pointer
    //  to the argument.
    const auto &castArgPtr = std::dynamic_pointer_cast<P>(arg);
    if (std::holds_alternative<NativeAction>(action)) {
      const auto &returnVal = (*std::get_if<NativeAction>(&action))(castArgPtr);
      if (std::holds_alternative<std::runtime_error>(returnVal)) {
        return { *std::get_if<std::runtime_error>(&returnVal) };
      }
      return { *std::get_if<ReturnPointer>(&returnVal) };
    }

    // Otherwise, temporarily define the parameter in the function's Context
    //  and evaluate the internal function code in that Context.
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

  // A function's "name" is a string representation of its type signature.
  static const std::string name;

  // getName() - Returns a function's type signature as a string.
  std::string getName() const {
      return name;
  }

  // Constructor(ast, context, param) - Creates a function with its internal
  //  Fleet code as ast, its Context as context, and the name of its parameter
  //  as param.
  FunctionValueBase(
    const TokenTree &ast, const Context::Pointer &context, std::string param
  ): action { ast }, evaluator { context }, isNative { false },
    paramName { param } {}

  // getIsNative() - Returns a boolean indicating whether the function should
  //  look like a native function to the code. Functions actually coded with
  //  native functions do not necessarily return true.
  bool getIsNative() const {
    return isNative;
  }

  // operator std::string() - Returns a string representation of the type.
  //  Returns "<Native [type]>" if the function should look native, or
  //  "<Function [type]>" if it should not.
  operator std::string() const {
    if (getIsNative()) {
      return std::string { "<Native " } + getName() + ">";
    }
    return std::string { "<Function " } + getName() + ">";
  }

  // Destructor - No special destructor is necessary.
  ~FunctionValueBase() = default;
};

// A normal FunctionValue (i.e. one that does not return a function) is the
//  same as a FunctionValueBase.
template <typename P, typename R>
class FunctionValue: public FunctionValueBase<P, R> {
  // Should inherit everything from FunctionValueBase
  using FunctionValueBase<P, R>::FunctionValueBase;
};

// A FunctionValue that returns a Function can be reversed, but is otherwise
//  the same as a FunctionValueBase.
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
  // getReverse() - Returns a FunctionValue that when called returns a function
  //  that takes parameters in the opposite order as the original function.
  Value::OrError getReverse() const {
    // Return a pointer to a FunctionValue created with a lambda function.
    //  This lamdba function takes the second parameter of the original function
    //  and returns another FunctionValue created with a lambda function. This
    //  lambda function takes the original first parameter and returns the
    //  result of evaluating the original function with the given parameters.
    return {
      Value::Pointer { new FunctionValue<P2, FunctionValue<P1, RFinal>> {
        [&] (const std::shared_ptr<P2> &secondParam) -> FirstReturn {
          return std::shared_ptr<FunctionValue<P1, RFinal>> {
          new FunctionValue<P1, RFinal> {
            [&, secondParam] (const std::shared_ptr<P1> &firstParam) ->
              FinalReturn {
              // Call the original function with the first parameter. If it
              //  returns an error, return that error.
              const auto &firstResultOrErr = this->call(firstParam);
              if (std::holds_alternative<std::runtime_error>(
                firstResultOrErr
              )) {
                return { *std::get_if<std::runtime_error>(&firstResultOrErr) };
              }
              const auto &firstResult = *std::get_if<Value::Pointer>(
                &firstResultOrErr
              );

              // Call the returned function with the second parameter. If it
              //  returns an error, return that error. Otherwise, return the
              //  resulting Value.
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

              // Ensure that the correct type is returned.
              if (!finalResultCast) {
                return FinalReturn { TypeError {
                  std::string { "Expected return value of type " } +
                  RFinal::name + " but got return value of type " +
                  finalResult->getName()
                } };
              }
              return FinalReturn { finalResultCast };
            },
            // Empty Contexts are used for evaluation since no Context is really
            //  needed for evaluation of these native functions.
            Context::Pointer { new Context {} },
            // Whether the reversed function is native is determined by whether
            //  the original function was native.
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

// Any FunctionValueBase's name (type signature) is the concatenation of its
//  parameter type, "->", and its return type
template <typename P, typename R>
const std::string FunctionValueBase<P, R>::name { P::name + "->" + R::name };

#endif
