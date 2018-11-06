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
  std::optional<const T> castValue() const {
    const auto result = dynamic_cast<const T*>(this);
    if (result == nullptr) {
      return {};
    }
    return { *result };
  }

  virtual operator std::string() const = 0;
  virtual OrError call(Pointer arg) const = 0;
  virtual std::string getName() const = 0;
  virtual ~Value() {};
};

#endif
