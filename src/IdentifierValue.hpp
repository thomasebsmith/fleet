// File: src/IdentifierValue.hpp
// Purpose: Header file for identifier Values (i.e. Values used on the left side
//  of the `=` operator and the left side of the `->` operator).

#ifndef IDENTIFIERVALUE_HPP
#define IDENTIFIERVALUE_HPP

#include <optional>
#include <string>
#include <vector>
#include "TokenTree.hpp"
#include "Value.hpp"

class IdentifierValue: public Value, public TokenTreeVisitor<std::optional<
  std::string>> {

private:
  TokenTree::TreePointer tree;
  mutable Value::Pointer tempValue;

public:
  // Constructor(ast) - Creates an IdentifierValue with a given TokenTree
  //  representation.
  IdentifierValue(const TokenTree &ast);

  // operator string() - Returns the string representation of the identifier.
  operator std::string() const;

  // call(arg) - Returns an error, since NumberValues cannot be called.
  Value::OrError call([[maybe_unused]] Value::Pointer arg) const;

  // name/getName() - Returns "Identifier", the name of an IdentifierValue-type
  //  value.
  static const std::string name;
  static std::string getClassName();
  std::string getName() const;

  // getIdentifier(value) - Returns the appropriate identifier string to set
  //  value to. May change value.
  std::optional<std::string> getIdentifier(Value::Pointer value);

  // Destructor - Default
  ~IdentifierValue() = default;

  // TokenTreeVisitor methods - Used internally to analyze the internal tree.
  std::optional<std::string> visit(const Token &token) const;
  std::optional<std::string> visit(
    const TokenTree &f, const TokenTree &x) const;
  std::optional<std::string> visit(const std::vector<TokenTree> &lines) const;
  std::optional<std::string> visit() const;
};

#endif
