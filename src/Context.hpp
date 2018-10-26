#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include "Value.hpp"

class Context {

public:
  typedef std::shared_ptr<Context> Pointer;
  typedef std::unordered_map<std::string, const Value::Pointer> ValueMap;

private:
  ValueMap values;
  Pointer parentContext;

public:
  Context();
  Context(Pointer parent);
  Context(ValueMap initialValues);
  Value::OrError getValue(const std::string &identifier);
  std::optional<std::runtime_error> define(
    const std::string &identifier, Value::Pointer value
  );
  Pointer getParentContext();
};

#endif
