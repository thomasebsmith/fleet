#include <functional>
#include <optional>
#include "Context.hpp"
#include "DefaultContext.hpp"
#include "FunctionValue.hpp"
#include "NumberValue.hpp"
#include "TypeError.hpp"
#include "Value.hpp"

const Context::Pointer DefaultContext::nativeContext { new Context {} };

Value::OrError DefaultContext::nativeAdd(Value::Pointer xPtr) {
  auto x = xPtr->castValue<NumberValue>();
  if (!x) {
    return { TypeError { "Argument to + must be of type Number" } };
  }
  return { Value::Pointer { new FunctionValue {
    FunctionValue::NativeAction {
      [x](Value::Pointer yPtr)->Value::OrError {
        auto y = yPtr->castValue<NumberValue>();
        if (!y) {
          return {
            TypeError { "Argument to ((+) _) must be of type Number " }
          };
        }
        return { Value::Pointer {
          new NumberValue { x->getRawNumber() + y->getRawNumber() }
        } };
      }
    },
    nativeContext
  } } };
}

DefaultContext::DefaultContext(): Context {
  Context::ValueMap {
    { "+", Value::Pointer {
      new FunctionValue {
        FunctionValue::NativeAction {
          &DefaultContext::nativeAdd
        },
        nativeContext
      }
    } }
  }
} {}
