// File: src/DefaultContext.hpp
// Purpose: Header file for DefaultContexts, which are simply Contexts
//  containing the default (built-in) global values. See src/DefaultContext.cpp
//  for method implementations.

#ifndef DEFAULTCONTEXT_HPP
#define DEFAULTCONTEXT_HPP

#include <functional>
#include <memory>
#include "Context.hpp"
#include "FunctionValue.hpp"
#include "NumberValue.hpp"
#include "Value.hpp"

// DefaultContext - Inherits from Context. It is simply a Context containing all
//  the built-in functions and values, such as `+`, `-`, `/`, and `::`.
class DefaultContext: public Context {
public:
  // A NativeBi is a function taking two Values and a Context and returning
  //  a Value.
  template <typename T1, typename T2, typename T3>
  using NativeBi = std::function<typename FunctionValue<T2, T3>::Return(
    const std::shared_ptr<T1> &,
    const std::shared_ptr<T2> &,
    const Context::Pointer &
  )>;

  template <typename T1, typename T2, typename T3>
  using NativeBiNoContext =
  std::function<typename FunctionValue<T2, T3>::Return(
    const std::shared_ptr<T1> &,
    const std::shared_ptr<T2> &
  )>;

private:
  // NOTE: Each call of a function creates a new lambda function encased in a
  //  FunctionValue. For example, 2 + 3 will evaluate to ((+) 2) 3, so (+) 2
  //  will be its own unique FunctionValue. This is probably inefficient and is
  //  not currently optimized.
  // This function creates a FunctionValue from a binary native function.
  template <typename T1, typename T2, typename T3>
  Value::Pointer createBiFunc(NativeBi<T1, T2, T3> func) {
    const auto lambdaFunc = [func](
      const std::shared_ptr<T1> &x,
      [[maybe_unused]] const Context::Pointer &context) {
      return typename FunctionValue<T1, FunctionValue<T2, T3>>::Return {
        typename FunctionValue<T1, FunctionValue<T2, T3>>::ReturnPointer {
          new FunctionValue<T2, T3> {
            typename FunctionValue<T2, T3>::NativeAction {
              [x, func](
                const std::shared_ptr<T2> &y,
                const Context::Pointer &context
              ) -> typename FunctionValue<T2, T3>::Return {
                return func(x, y, context);
              }
            },
            Context::Pointer { context }
          }
        }
      };
    };
    return Value::Pointer {
      new FunctionValue<T1, FunctionValue<T2, T3>> {
        typename FunctionValue<T1, FunctionValue<T2, T3>>::NativeAction {
          lambdaFunc
        },
        // Create a *RAW POINTER* since a shared_ptr will be created by the
        //  object that owns this DefaultContext.
        Context::Pointer { this, true }
      }
    };
  }

  template <typename T1, typename T2, typename T3>
  Value::Pointer createBiFunc(NativeBiNoContext<T1, T2, T3> func) {
    return createBiFunc<T1, T2, T3>([func](
      const std::shared_ptr<T1> &x,
      const std::shared_ptr<T2> &y,
      [[maybe_unused]] const Context::Pointer &ignored) {
        return func(x, y);
    });
  }

  const Value::Pointer add;
  const Value::Pointer multiply;
  const Value::Pointer pow;
  const Value::Pointer set;

  public:
  // DefaultContext - The default constructor. Creates a Context with all the
  //  default functions and values included.
  DefaultContext();
};

#endif
