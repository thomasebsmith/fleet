#ifndef VALUE_HPP
#define VALUE_HPP

#include <memory>
#include <optional>
#include <stdexcept>
#include <variant>

class Value {
public:
  typedef std::shared_ptr<Value> Pointer;
  typedef std::variant<std::runtime_error, Pointer> OrError;

  template <typename T>
  std::optional<T> castValue() const;

  virtual OrError call(Pointer arg) const = 0;
  virtual ~Value() {};
};

#endif
