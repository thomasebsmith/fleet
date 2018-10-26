#ifndef DEFAULTCONTEXT_HPP
#define DEFAULTCONTEXT_HPP

#include "Context.hpp"
#include "Value.hpp"

class DefaultContext: public Context {
private:
  static const Context::Pointer nativeContext;
  static Value::OrError nativeAdd(Value::Pointer x);

public:
  DefaultContext();
};

#endif
