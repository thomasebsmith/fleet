// File: src/Type.hpp
// Purpose: Header file for Type Values, which can be created at runtime.
#ifndef TYPE_HPP
#define TYPE_HPP

#include <functional>
#include <optional>
#include <variant>
#include "Value.hpp"

class Type: public Value {
public:
  typedef std::function<bool(Value::Pointer)> SimpleMatcher;
  typedef std::function<bool(Value::Pointer, std::string)> UUIDMatcher;
private:
  std::string instanceName;
  std::variant<SimpleMatcher, UUIDMatcher> matcher;
  std::optional<std::string> typeUUID;
public:
  // Constructor(name, matches) - Creates a Type that simply uses a function to
  //  check whether a given Value is of the Type.
  Type(std::string name, SimpleMatcher matches);

  // Constructor(name, matches, uuid) - Creates a Type with a matching function
  //  and a string UUID that internally differentiates among custom types.
  Type(std::string name, UUIDMatcher matches, std::string uuid);

  // Destructor - Default
  ~Type() = default;

  // call(arg) - Returns an error, since Types cannot be called.
  Value::OrError call([[maybe_unused]] Value::Pointer arg) const;

  // operator string() - Returns the name of the Type and its UUID if it exists.
  operator std::string() const;

  // contains(value) - Returns true iff value is an instance of this Type.
  bool contains(Value::Pointer value) const;

  // name/getName() - Returns "Type".
  static const std::string name;
  static std::string getClassName();
  std::string getName() const;

  // fromStatic<T>() - Returns a Type that conforms to the C++ type T. T should
  //  be a subclass of Value.
  template <typename T>
  static Type fromStatic() {
    return Type {
      T::name,
      [] (Value::Pointer val) -> bool {
        return val->canCastValue<T>();
      }
    };
  }
};

#endif
