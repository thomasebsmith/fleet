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

// A Context::Pointer to an empty context is stored to be used for native
//  operations. Native operations do not need a Context (since they use native
//  variables rather than Contexts), so this is used.
const Context::Pointer DefaultContext::nativeContext { new Context {} };

// This function is defined as `+` in DefaultContexts. It takes a Pointer to
//  a NumberValue and returns a FunctionValue which takes another NumberValue
//  and returns a pointer containing a number value which is the sum
//  of those two numbers.
const Value::Pointer DefaultContext::add =
DefaultContext::createBiFunc<NumberValue, NumberValue, NumberValue>([](
  const std::shared_ptr<NumberValue> &x,
  const std::shared_ptr<NumberValue> &y) ->
  FunctionValue<NumberValue, NumberValue>::ReturnPointer {
  return { FunctionValue<NumberValue, NumberValue>::ReturnPointer {
    new NumberValue { x->getRawNumber() + y->getRawNumber() }
  } };
});

// This function is defined as `*` in DefaultContexts. It returns the product
//  of its two arguments.
const Value::Pointer DefaultContext::multiply =
  DefaultContext::createBiFunc<NumberValue, NumberValue, NumberValue>([](
    const std::shared_ptr<NumberValue> &x,
    const std::shared_ptr<NumberValue> &y) ->
    FunctionValue<NumberValue, NumberValue>::ReturnPointer {
  return { FunctionValue<NumberValue, NumberValue>::ReturnPointer {
    new NumberValue { x->getRawNumber() * y->getRawNumber() }
  } };
});

// This function is defined as `^` in DefaultContexts. It returns its first
//  argument raised to the power of its second argument.
const Value::Pointer DefaultContext::pow =
  DefaultContext::createBiFunc<NumberValue, NumberValue, NumberValue>([](
    const std::shared_ptr<NumberValue> &x,
    const std::shared_ptr<NumberValue> &y) ->
    FunctionValue<NumberValue, NumberValue>::ReturnPointer {
  return { FunctionValue<NumberValue, NumberValue>::ReturnPointer {
    new NumberValue { std::pow(x->getRawNumber(), y->getRawNumber()) }
  } };
});

// This function is defined as `=` in DefaultContexts. It sets its first
//  argument to be equal to its second argument.
const Value::Pointer DefaultContext::set =
  DefaultContext::createBiFunc<IdentifierValue, Value, Value>([](
    const std::shared_ptr<IdentifierValue> &id,
    const Value::Pointer &value,
    const Context::Pointer &context) ->
    Value::Pointer {
  context->define(id, value);
  return value;
});

// The default constructor creates a Context containing all the default values.
DefaultContext::DefaultContext(): Context {
  Context::ValueMap {
    { "+", DefaultContext::add },
    { "*", DefaultContext::multiply },
    { "^", DefaultContext::pow },
    { "=", DefaultContext::set }
  }
} {}
