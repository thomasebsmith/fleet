#include "TestContext.hpp"
#include "Context.hpp"
#include "NumberValue.hpp"
#include "Tester.hpp"
#include "Value.hpp"

// Function prototypes
void testGetValueSimple();
void testGetNonexistentValue();

int TestContext::main() {
  Tester tester("Context tests");
  tester.test("Simple getValue()", testGetValueSimple);
  tester.test("Get nonexistent value", testGetNonexistentValue);
  return tester.run();
}

void testGetValueSimple() {
  Value::Pointer value { new NumberValue { 3.52 } };
  Context context {
    Context::ValueMap {
      { "someIdentifier", value }
    }
  };
  Value::OrError result = context.getValue("someIdentifier");
  Tester::confirm(std::holds_alternative<Value::Pointer>(result));
  Tester::confirm(*std::get_if<Value::Pointer>(&result) == value);
}

void testGetNonexistentValue() {
  Value::Pointer value { new NumberValue { 999.8876 } };
  Value::Pointer value2 { new NumberValue { 0.0 } };
  Context context {
    Context::ValueMap {
      { "foo", value },
      { "foo__", value2 }
    }
  };
  Value::OrError result = context.getValue("nonexistent");
  Tester::confirm(std::holds_alternative<std::runtime_error>(result));

  Value::OrError result2 = context.getValue("foo_");
  Tester::confirm(std::holds_alternative<std::runtime_error>(result2));
}
