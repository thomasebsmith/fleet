#include <functional>
#include <memory>
#include <optional>
#include "Context.hpp"
#include "DefaultContext.hpp"
#include "FunctionValue.hpp"
#include "NumberValue.hpp"
#include "TypeError.hpp"
#include "Value.hpp"

const Context::Pointer DefaultContext::nativeContext { new Context {} };

DefaultContext::BiNumberFunc::Return DefaultContext::nativeAdd(
  const std::shared_ptr<NumberValue> &x
) {
  return { DefaultContext::BiNumberFunc::ReturnPointer {
    new DefaultContext::NumberFunc {
      DefaultContext::NumberFunc::NativeAction {
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

DefaultContext::DefaultContext(): Context {
  Context::ValueMap {
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
