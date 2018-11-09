// File: src/DefaultContext.cpp
// Purpose: Source file for DefaultContexts, which are simply Contexts
//  containing the default (built-in) global values. See src/DefaultContext.hpp
//  for more documentation.

#include <functional>
#include <memory>
#include <optional>
#include "Context.hpp"
#include "DefaultContext.hpp"
#include "FunctionValue.hpp"
#include "NumberValue.hpp"
#include "TypeError.hpp"
#include "Value.hpp"

// A Context::Pointer to an empty context is stored to be used for native
//  operations. Native operations do not need a Context (since they use native
//  variables rather than Contexts), so this is used.
const Context::Pointer DefaultContext::nativeContext { new Context {} };

// This function is defined as `+` in DefaultContexts. It takes a Pointer to
//  a NumberValue and returns a FunctionValue which takes another NumberValue
//  and returns a pointer containing a number value which is the sum
//  of those two numbers.
DefaultContext::BiNumberFunc::Return DefaultContext::nativeAdd(
  const std::shared_ptr<NumberValue> &x
) {
  // Each call of this function creates a new lambda function encased in a
  //  FunctionValue. For example, 2 + 3 will evaluate to ((+) 2) 3, so (+) 2
  //  will be its own unique FunctionValue. This is probably inefficient and is
  //  not currently optimized.
  return { DefaultContext::BiNumberFunc::ReturnPointer {
    new DefaultContext::NumberFunc {
      DefaultContext::NumberFunc::NativeAction {
        // This lambda function captures the first parameter `x` and returns
        //  the sum of `x` and its argument `y`.
        [x](const std::shared_ptr<NumberValue> &y) ->
        DefaultContext::NumberFunc::Return {
          return { DefaultContext::NumberFunc::ReturnPointer {
            new NumberValue { x->getRawNumber() + y->getRawNumber() }
          } };
        }
      },
      nativeContext
    }
  } };
}

// The default constructor creates a Context containing all the default values.
DefaultContext::DefaultContext(): Context {
  Context::ValueMap {
    // + is defined in nativeAdd
    { "+", Value::Pointer {
      new DefaultContext::BiNumberFunc {
        DefaultContext::BiNumberFunc::NativeAction {
          &DefaultContext::nativeAdd
        },
        nativeContext
      }
    } }
  }
} {}
