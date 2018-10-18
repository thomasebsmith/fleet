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
std::variant<std::runtime_error, Value::Pointer> Context::getValue(
  const std::string &identifier
) {
  auto iterator = values.find(identifier);
  if (iterator == values.end()) {
    return { TypeError { identifier + " is undefined" } };
  }
  return { iterator->second };
}

std::optional<std::runtime_error> Context::define(
  const std::string &identifier, const Value &value
) {
  auto iterator = values.find(identifier);
  if (iterator != values.end()) {
    return { TypeError { identifier + " is already defined" } };
  }
  Value::Pointer ptr = value.clone();
  values.insert({
    { identifier, ptr }
  });
  return {};
}
