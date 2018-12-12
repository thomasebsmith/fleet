// File: src/Value.hpp
// Purpose: Header file for an abstract class from which all Fleet Values should
//  inherit. Examples of Values include NumberValues, FunctionValues, etc.

#ifndef VALUE_HPP
#define VALUE_HPP

#include <memory>
#include <optional>
#include <stdexcept>
#include <variant>

class Value {
public:
  // Value::Pointer represents a generic smart pointer to a Value. Useful for
  //  generic structures containing Values of unknown types.
  typedef std::shared_ptr<Value> Pointer;

  // Value::OrError represents a runtime_error or a Value::Pointer. Useful for
  //  return types of functions that may return Values or errors.
  typedef std::variant<std::runtime_error, Pointer> OrError;
  
  // castValue<T>() - Returns an optional, which contains a value of type T iff
  //  the Value is internally of that type.
  template <typename T>
  std::optional<const T> castValue() const {
    const T *result = dynamic_cast<const T *>(this);
    if (result == nullptr) {
      return {};
    }
    return { *result };
  }

  // virtual std::string() - Returns the string representation of the Value.
  virtual operator std::string() const = 0;

  // virtual call(arg) - Returns the result of calling the Value with the given
  //  argument, or an error if not appropriate.
  virtual OrError call(Pointer arg) const = 0;

  // virtual getName() - Returns the name of the type (e.g. Number, String).
  virtual std::string getName() const = 0;

  // virtual ~Value() - Virtual destructor. Must be implemented by subclasses.
  virtual ~Value() {};
};

#endif
