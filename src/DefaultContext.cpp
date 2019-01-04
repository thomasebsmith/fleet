// File: src/DefaultContext.cpp
// Purpose: Source file for DefaultContexts, which are simply Contexts
//  containing the default (built-in) global values. See src/DefaultContext.hpp
//  for more documentation.

#include <cmath>
#include <functional>
#include <memory>
#include <optional>
#include "DefaultContext.hpp"
#include "Context.hpp"
#include "FunctionValue.hpp"
#include "IdentifierValue.hpp"
#include "NumberValue.hpp"
#include "TypeError.hpp"
#include "Value.hpp"

// The default constructor creates a Context containing all the default values.
DefaultContext::DefaultContext():
  // This function is defined as `+` in DefaultContexts. It takes a Pointer to
  //  a NumberValue and returns a FunctionValue which takes another NumberValue
  //  and returns a pointer containing a number value which is the sum
  //  of those two numbers.
  add {
    createBiFunc<NumberValue, NumberValue, NumberValue>([](
    const std::shared_ptr<NumberValue> &x,
    const std::shared_ptr<NumberValue> &y) ->
    FunctionValue<NumberValue, NumberValue>::Return {
    return { FunctionValue<NumberValue, NumberValue>::Return {
      FunctionValue<NumberValue, NumberValue>::ReturnPointer {
        new NumberValue { x->getRawNumber() + y->getRawNumber() }
      }
    } };
    })
  },

  // This function is defined as `*` in DefaultContexts. It returns the product
  //  of its two arguments.
  multiply {
    createBiFunc<NumberValue, NumberValue, NumberValue>([](
      const std::shared_ptr<NumberValue> &x,
      const std::shared_ptr<NumberValue> &y) ->
      FunctionValue<NumberValue, NumberValue>::Return {
    return { FunctionValue<NumberValue, NumberValue>::Return {
      FunctionValue<NumberValue, NumberValue>::ReturnPointer {
        new NumberValue { x->getRawNumber() * y->getRawNumber() }
      }
    } };
    })
  },

  // This function is defined as `^` in DefaultContexts. It returns its first
  //  argument raised to the power of its second argument.
  pow {
    createBiFunc<NumberValue, NumberValue, NumberValue>([](
      const std::shared_ptr<NumberValue> &x,
      const std::shared_ptr<NumberValue> &y) ->
      FunctionValue<NumberValue, NumberValue>::Return {
    return { FunctionValue<NumberValue, NumberValue>::Return {
      FunctionValue<NumberValue, NumberValue>::ReturnPointer {
        new NumberValue { std::pow(x->getRawNumber(), y->getRawNumber()) }
      }
    } };
    })
  },

  // This function is defined as `=` in DefaultContexts. It sets its first
  //  argument to be equal to its second argument.
  set {
    createBiFunc<IdentifierValue, Value, Value>([](
      const std::shared_ptr<IdentifierValue> &id,
      const Value::Pointer &value,
      const Context::Pointer &context) ->
      Value::OrError {
    auto err = context->define(id, value);
    if (err) {
      return *err;
    }
    return value;
    })
  }
{
  define("+", DefaultContext::add);
  define("*", DefaultContext::multiply);
  define("^", DefaultContext::pow);
  define("=", DefaultContext::set);
}
