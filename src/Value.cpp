// File: src/Value.hpp
// Purpose: Source file for an abstract class from which all Fleet Values should
//  inherit. Examples of Values include NumberValues, FunctionValues, etc.

#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <variant>
#include "Value.hpp"

const std::string Value::name { "Value" };

std::string Value::getClassName() {
  return Value::name;
}
