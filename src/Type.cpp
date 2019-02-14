// File: src/Type.cpp
// Purpose: Source file for Type Values, which can be created at runtime.

#include <functional>
#include <optional>
#include <variant>
#include "Type.hpp"
#include "Value.hpp"

// Constructor(name, matches) - Creates a Type that simply uses a function to
//  check whether a given value is of the Type.
Type::Type(std::string name, Type::SimpleMatcher matches):
  instanceName { name }, matcher { matches }, typeUUID {} {}

// Constructor(name, matches, uuid) - Creates a Type with a matching function
//  and a string UUID that internally differentiates among custom types.
Type::Type(std::string name, Type::UUIDMatcher matches, std::string uuid):
  instanceName { name }, matcher { matches }, typeUUID { uuid } {}

// call(arg) - Returns an error, since Types cannot be called.
Value::OrError Type::call([[maybe_unused]] Value::Pointer arg) const {
  return { std::runtime_error {
    std::string { "Value of Type " } + name + " cannot be called."
  } };
}

// operator string() - Returns the name of the Type and its UUID if it exists.
Type::operator std::string() const {
  if (!typeUUID) {
    return instanceName;
  }
  return instanceName + "(" + *typeUUID + ")";
}

// contains(value) - Returns true iff value is an instance of this Type.
bool Type::contains(Value::Pointer value) const {
  if (!typeUUID) {
    return (*std::get_if<SimpleMatcher>(&matcher))(value);
  }
  else {
    return (*std::get_if<UUIDMatcher>(&matcher))(value, *typeUUID);
  }
}

// name/getName() - Returns "Type".
const std::string Type::name = "Type";
std::string Type::getClassName() {
  return Type::name;
}
std::string Type::getName() const {
  return Type::name;
}
