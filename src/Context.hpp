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

private:
  std::unordered_map<std::string, const Value::Pointer> values;
  Pointer parentContext;

public:
  Context();
  Context(Pointer parent);
  std::variant<std::runtime_error, Value::Pointer> getValue(
    const std::string &identifier
  );
  std::optional<std::runtime_error> define(
    const std::string &identifier, const Value &value
  );
};

#endif
