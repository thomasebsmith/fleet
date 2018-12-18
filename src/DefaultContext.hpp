// File: src/DefaultContext.hpp
// Purpose: Header file for DefaultContexts, which are simply Contexts
//  containing the default (built-in) global values. See src/DefaultContext.cpp
//  for method implementations.

#ifndef DEFAULTCONTEXT_HPP
#define DEFAULTCONTEXT_HPP

#include <memory>
#include "Context.hpp"
#include "FunctionValue.hpp"
#include "NumberValue.hpp"
#include "Value.hpp"

// DefaultContext - Inherits from Context. It is simply a Context containing all
//  the built-in functions and values, such as `+`, `-`, `/`, and `::`.
class DefaultContext: public Context {
public:
  // NumberFunc is used as a type alias for FunctionValues that take a number
  //  and return a number (e.g. `(+) 58`).
  typedef FunctionValue<NumberValue, NumberValue> NumberFunc;

  // BiNumberFunc is used as a type alias for FunctionValues that take two
  //  numbers and return a number (e.g. `+`).
  typedef FunctionValue<NumberValue, NumberFunc> BiNumberFunc;

  // A NativeBiNumber is a function taking two NumberValues and returning
  //  another one.
  typedef std::function<NumberFunc::ReturnPointer(
    const std::shared_ptr<NumberValue> &,
    const std::shared_ptr<NumberValue> &
  )> NativeBiNumber;

private:
  static const Context::Pointer nativeContext;
  static Value::Pointer createBiNumberFunc(NativeBiNumber func);
  static const Value::Pointer add;
  static const Value::Pointer multiply;
  static const Value::Pointer pow;

public:
  // DefaultContext - The default constructor. Creates a Context with all the
  //  default functions and values included.
  DefaultContext();
};

#endif
