// File: src/IdentifierValue.cpp
// Purpose: Source file for identifier Values (i.e. Values used on the left side
//  of the `=` operator and the left side of the `->` operator).

#include <string>
#include "IdentifierValue.hpp"
#include "TokenTree.hpp"
#include "Value.hpp"

// Constructor
IdentifierValue::IdentifierValue(TokenTree::TreePointer ast): tree { ast } {}

// operator string() - Returns the string representation of the internal token
//  tree.
IdentifierValue::operator std::string() const {
  return static_cast<std::string>(*tree);
}

// call([unused] arg) - Returns an error, since NumberValues cannot be called.
Value::OrError IdentifierValue::call(
  [[maybe_unused]] Value::Pointer arg
) const {
  return { std::runtime_error { std::string {
    "Value of type "
  } + name + " cannot be called" } };
}

const std::string IdentifierValue::name { "Identifier" };

// getName() - Returns "Identifier".
std::string IdentifierValue::getName() const {
  return IdentifierValue::name;
}

std::string IdentifierValue::getClassName() {
  return IdentifierValue::name;
}
