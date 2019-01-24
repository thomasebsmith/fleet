#include <unordered_map>
#include "TestContext.hpp"
#include "Context.hpp"
#include "NumberValue.hpp"
#include "Tester.hpp"
#include "Value.hpp"

// Function prototypes
void testGetValueSimple();

int TestContext::main() {
  Tester tester("Context tests");
  tester.test("Simple getValue()", testGetValueSimple);
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
