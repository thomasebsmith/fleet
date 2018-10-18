#include <optional>
#include "Value.hpp"

template <typename T>
std::optional<T> Value::castValue() const {
  auto result = dynamic_cast<T*>(this);
  if (result == nullptr) {
    return {};
  }
  return { *result };
}
