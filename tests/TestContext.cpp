#include "TestContext.hpp"
#include "Context.hpp"
#include "NumberValue.hpp"
#include "Tester.hpp"
#include "Value.hpp"

// Function prototypes
void testGetValueSimple();
void testGetNonexistentValue();
void testGetTwoValues();
void testNestedContext();
void testNestedContext2();

int TestContext::main() {
  Tester tester("Context tests");
  tester.test("Simple getValue()", testGetValueSimple);
  tester.test("Get nonexistent value", testGetNonexistentValue);
  tester.test("Get two values", testGetTwoValues);
  tester.test("Nested contexts", testNestedContext);
  tester.test("Nested contexts 2", testNestedContext2);
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

void testGetTwoValues() {
  Value::Pointer value { new NumberValue { 0.001 } };
  Value::Pointer value2 { new NumberValue { 124536.0 } };
  Context context {
    Context::ValueMap {
      { "this_is_a_test", value },
      { "$", value2 }
    }
  };
  Value::OrError result = context.getValue("this_is_a_test");
  Tester::confirm(std::holds_alternative<Value::Pointer>(result));
  Tester::confirm(*std::get_if<Value::Pointer>(&result) == value);
  Value::OrError result2 = context.getValue("$");
  Tester::confirm(std::holds_alternative<Value::Pointer>(result2));
  Tester::confirm(*std::get_if<Value::Pointer>(&result2) == value2);
}

void testNestedContext() {
  Value::Pointer value { new NumberValue { 101.202 } };
  Context::Pointer parent = new Context {
    Context::ValueMap {
      { "foobar", value }
    }
  };
  Context child { parent };
  Value::OrError result = child.getValue("foobar");
  Tester::confirm(std::holds_alternative<Value::Pointer>(result));
  Tester::confirm(*std::get_if<Value::Pointer>(&result) == value);
}

void testNestedContext2() {
  Value::Pointer value { new NumberValue { 1.23 } };
  Context::Pointer parent = new Context {};
  Context child { parent };
  child.define("...", value);
  Value::OrError result = child.getValue("...");
  Tester::confirm(std::holds_alternative<Value::Pointer>(result));
  Tester::confirm(*std::get_if<Value::Pointer>(&result) == value);
}
