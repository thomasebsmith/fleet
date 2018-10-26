#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "Context.hpp"
#include "TypeError.hpp"
#include "Value.hpp"

Context::Context() {}
Context::Context(Context::Pointer parent): parentContext(parent) {}
Context::Context(Context::ValueMap initialValues): values { initialValues } {}
Value::OrError Context::getValue(const std::string &identifier) {
  auto iterator = values.find(identifier);
  if (iterator == values.end()) {
    if (parentContext) {
      return parentContext->getValue(identifier);
    }
    return { TypeError { identifier + " is undefined" } };
  }
  return { iterator->second };
}

std::optional<std::runtime_error> Context::define(
  const std::string &identifier, Value::Pointer value
) {
  auto iterator = values.find(identifier);
  if (iterator != values.end()) {
    return { TypeError { identifier + " is already defined" } };
  }
  values.insert({
    { identifier, value }
  });
  return {};
}

Context::Pointer Context::getParentContext() {
  return parentContext;
}
