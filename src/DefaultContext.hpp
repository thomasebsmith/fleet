#ifndef DEFAULTCONTEXT_HPP
#define DEFAULTCONTEXT_HPP

#include "Context.hpp"
#include "FunctionValue.hpp"
#include "NumberValue.hpp"
#include "Value.hpp"

class DefaultContext: public Context {
public:
  typedef FunctionValue<NumberValue, NumberValue> NumberFunc;
  typedef FunctionValue<NumberValue, NumberFunc> BiNumberFunc;
private:
  static const Context::Pointer nativeContext;
  static BiNumberFunc::Return nativeAdd(const NumberValue &x);

public:
  DefaultContext();
};

#endif