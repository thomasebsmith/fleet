// File: src/DefaultContext.cpp
// Purpose: Source file for DefaultContexts, which are simply Contexts
//  containing the default (built-in) global values. See src/DefaultContext.hpp
//  for more documentation.

#include <functional>
#include <memory>
#include <optional>
#include "DefaultContext.hpp"
#include "Context.hpp"
#include "FunctionValue.hpp"
#include "NumberValue.hpp"
#include "TypeError.hpp"
#include "Value.hpp"

// A Context::Pointer to an empty context is stored to be used for native
//  operations. Native operations do not need a Context (since they use native
//  variables rather than Contexts), so this is used.
const Context::Pointer DefaultContext::nativeContext { new Context {} };

// Each call of a BiNumberFunc creates a new lambda function encased in a
//  FunctionValue. For example, 2 + 3 will evaluate to ((+) 2) 3, so (+) 2
//  will be its own unique FunctionValue. This is probably inefficient and is
//  not currently optimized.
Value::Pointer DefaultContext::createBiNumberFunc(
  DefaultContext::NativeBiNumber func) {
  const auto lambdaFunc = [func](const std::shared_ptr<NumberValue> &x) {
    return DefaultContext::BiNumberFunc::ReturnPointer {
      new DefaultContext::NumberFunc {
        DefaultContext::NumberFunc::NativeAction {
          [x, func](const std::shared_ptr<NumberValue> &y) ->
          DefaultContext::NumberFunc::Return {
            return func(x, y);
          }
        },
        nativeContext
      }
    };
  };
  return Value::Pointer {
    new DefaultContext::BiNumberFunc {
      DefaultContext::BiNumberFunc::NativeAction {
        lambdaFunc
      },
      nativeContext
    }
  };
}

// This function is defined as `+` in DefaultContexts. It takes a Pointer to
//  a NumberValue and returns a FunctionValue which takes another NumberValue
//  and returns a pointer containing a number value which is the sum
//  of those two numbers.
Value::Pointer DefaultContext::add = DefaultContext::createBiNumberFunc([](
  const std::shared_ptr<NumberValue> &x,
  const std::shared_ptr<NumberValue> &y) ->
  DefaultContext::NumberFunc::ReturnPointer {
  return { DefaultContext::NumberFunc::ReturnPointer {
    new NumberValue { x->getRawNumber() + y->getRawNumber() }
  } };
});

// The default constructor creates a Context containing all the default values.
DefaultContext::DefaultContext(): Context {
  Context::ValueMap {
    { "+", DefaultContext::add }
  }
} {}
