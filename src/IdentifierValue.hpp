// File: src/IdentifierValue.hpp
// Purpose: Header file for identifier Values (i.e. Values used on the left side
//  of the `=` operator and the left side of the `->` operator).

#ifndef IDENTIFIERVALUE_HPP
#define IDENTIFIERVALUE_HPP

#include <string>
#include "TokenTree.hpp"
#include "Value.hpp"

class IdentifierValue: public Value {

private:
  TokenTree::TreePointer tree;

public:
  // Constructor(ast) - Creates an IdentifierValue with a given TokenTree
  //  representation.
  IdentifierValue(TokenTree::TreePointer ast);

  // operator string() - Returns the string representation of the identifier.
  operator std::string() const;

  // call(arg) - Returns an error, since NumberValues cannot be called.
  Value::OrError call([[maybe_unused]] Value::Pointer arg) const;

  // getName() - Returns "Identifier", the name of an IdentifierValue-type
  //  value.
  static const std::string name;
  std::string getName() const;

  // Destructor - Default
  ~IdentifierValue() = default;
};

#endif
