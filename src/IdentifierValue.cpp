// File: src/IdentifierValue.cpp
// Purpose: Source file for identifier Values (i.e. Values used on the left side
//  of the `=` operator and the left side of the `->` operator).

#include <optional>
#include <string>
#include <vector>
#include "IdentifierValue.hpp"
#include "TokenTree.hpp"
#include "Value.hpp"

// Constructor
IdentifierValue::IdentifierValue(const TokenTree &ast): tree { new TokenTree { ast } } {}

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

// getIdentifier(value) - Returns an appropriate string to be used to store
//  value. May change value to match.
std::optional<std::string> IdentifierValue::getIdentifier(
  Value::Pointer value
) {
  tempValue = Value::Pointer { value };
  auto result = tree->accept(*this);
  tempValue = Value::Pointer { nullptr };
  return result;
}

// visit(value) - Changes tempValue and returns a string representing the
//  identifier contained within a given TokenTree.
std::optional<std::string> IdentifierValue::visit(const Token &token) const {
  if (token.getType() != Token::Type::Identifier) {
    return {};
  }
  return token.getValue();
}

std::optional<std::string> IdentifierValue::visit(
  [[maybe_unused]] const TokenTree &f, [[maybe_unused]] const TokenTree &x
) const {
  return {};
}

std::optional<std::string> IdentifierValue::visit(
  [[maybe_unused]] const std::vector<TokenTree> &lines
) const {
  return {};
}

std::optional<std::string> IdentifierValue::visit() const {
  return {};
}
