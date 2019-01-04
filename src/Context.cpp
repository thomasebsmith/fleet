// File: src/Context.cpp
// Purpose: Source file for Contexts, which are used to hold values associated
//  with variable names. See src/Context.hpp for more information and
//  documentation.

#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "Context.hpp"
#include "IdentifierValue.hpp"
#include "TypeError.hpp"
#include "Value.hpp"

// Constructors
Context::Context() {}
Context::Context(Context::Pointer parent): parentContext(parent) {}
Context::Context(Context::ValueMap initialValues): values { initialValues } {}

// Methods

// getValue(identifier) returns either a pointer to a Value (if the value exists
//  in the current or a parent context) or an error (if the value does not
//  exist).
Value::OrError Context::getValue(const std::string &identifier) {
  auto iterator = values.find(identifier);
  if (iterator == values.end()) {
    // If the value is not found, either look in the parent context or
    //  return an error if there is no parent context.`
    if (parentContext) {
      return parentContext->getValue(identifier);
    }
    return { TypeError { identifier + " is undefined" } };
  }
  // If the iterator is not at `end`, the value was found, so return that value.
  return { iterator->second };
}

// define(identifier, value) defines value in the current context with a name of
//  identifier. If the value already exists, it returns an error. Otherwise,
//  it returns an empty optional.
std::optional<std::runtime_error> Context::define(
  const std::string &identifier, Value::Pointer value
) {
  auto iterator = values.find(identifier);
  if (iterator != values.end()) {
    // If the value is already defined, it cannot be redefined.
    return { TypeError { identifier + " is already defined" } };
  }
  // If the value is not already defined, add it to the internal value map.
  values.insert({
    { identifier, value }
  });
  return {};
}

std::optional<std::runtime_error> Context::define(
  const std::shared_ptr<IdentifierValue> &identifier, Value::Pointer value
) {
  auto idString = identifier->getIdentifier(value);
  if (!idString) {
    return { TypeError {
      static_cast<std::string>(*identifier) + " is not a valid identifier"
    } };
  }
  return define(*idString, value);
}
// getParentContext() returns a pointer to the parent context - i.e. the context
//  containing this one. This *can* return a *nil pointer* if there is no parent
//  context!!
Context::Pointer Context::getParentContext() const {
  return parentContext;
}
