// File: src/Context.hpp
// Purpose: Header file for Contexts, which are used to hold values associated
//  with variable names. See src/Context.cpp for method implementations.

#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include "IdentifierValue.hpp"
#include "Value.hpp"

class Context {

public:
  // Context::Pointer and Context::ValueMap can be used as type aliases for
  //  the internal ways contexts and value maps are stored in this class.
  typedef std::shared_ptr<Context> Pointer;
  typedef std::unordered_map<std::string, const Value::Pointer> ValueMap;

private:
  ValueMap values;
  Pointer parentContext;

public:
  // Constructors

  // Context() - The default constructor. Creates a Context with no initial
  //  identifiers defined and no parent Context.
  Context();

  // Context(parent) - Creates a Context with no initial identifiers defined but
  //  with a parent Context as specified.
  Context(Pointer parent);

  // Context(initialValues) - Creates a Context with initial values defined
  //  according to the ValueMap but with no parent context.
  Context(ValueMap initialValues);

  // Methods

  // getValue(identifier) - Returns a Value::Pointer if identifier is defined in
  //  this Context or a parent Context, or an error if it is not defined.
  Value::OrError getValue(const std::string &identifier);

  // define(identifier, value) - Adds an entry to the internal map of values
  //  with a name of identifier if identifier is not already defined in *this*
  //  context. If identifier is already defined, it returns an error. Otherwise,
  //  it returns an empty optional.
  std::optional<std::runtime_error> define(
    const std::string &identifier, Value::Pointer value
  );

  std::optional<std::runtime_error> define(
    const std::shared_ptr<IdentifierValue> &identifier, Value::Pointer value
  );

  // getParentContext() - Returns a Context::Pointer to the parent Context (i.e.
  //  the Context containing this one). DANGER: This method *can* return a *nil
  //  pointer* if there is no parent context!!
  Pointer getParentContext() const;
};

#endif
