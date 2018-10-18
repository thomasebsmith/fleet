#ifndef VALUE_HPP
#define VALUE_HPP

#include <memory>
#include <optional>
#include <stdexcept>
#include <variant>

class Value {
public:
  typedef std::shared_ptr<Value> Pointer;

  template <typename T>
  std::optional<T> castValue() const;

  virtual std::variant<std::runtime_error, Value::Pointer> call(
    Value::Pointer arg
  ) const = 0;
  virtual Pointer clone() const = 0;
  virtual ~Value() = 0;
};

#endif
