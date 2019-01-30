#include <memory>
#include <variant>
#include "TestContext.hpp"
#include "Context.hpp"
#include "IdentifierValue.hpp"
#include "NumberValue.hpp"
#include "Tester.hpp"
#include "Token.hpp"
#include "Value.hpp"

// Function prototypes
bool valuesEqual(Value::OrError result, Value::Pointer expected);
void testGetValueSimple();
void testGetNonexistentValue();
void testGetTwoValues();
void testNestedContext();
void testNestedContext2();
void testNestedContext3();
void testIdentifierDefine();

int TestContext::main() {
  Tester tester("Context tests");
  tester.test("Simple getValue()", testGetValueSimple);
  tester.test("Get nonexistent value", testGetNonexistentValue);
  tester.test("Get two values", testGetTwoValues);
  tester.test("Nested contexts", testNestedContext);
  tester.test("Nested contexts 2", testNestedContext2);
  tester.test("Nested contexts 3", testNestedContext3);
  tester.test("Identifier define", testIdentifierDefine);
  return tester.run();
}

bool valuesEqual(Value::OrError result, Value::Pointer expected) {
  if (!std::holds_alternative<Value::Pointer>(result)) {
    return false;
  }
  return *std::get_if<Value::Pointer>(&result) == expected;
}

void testGetValueSimple() {
  Value::Pointer value { new NumberValue { 3.52 } };
  Context context {
    Context::ValueMap {
      { "someIdentifier", value }
    }
  };
  Value::OrError result = context.getValue("someIdentifier");
  Tester::confirm(valuesEqual(result, value));
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
  Tester::confirm(valuesEqual(result, value));
  Value::OrError result2 = context.getValue("$");
  Tester::confirm(valuesEqual(result2, value2));
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
  Tester::confirm(valuesEqual(result, value));
}

void testNestedContext2() {
  Value::Pointer value { new NumberValue { 1.23 } };
  Context::Pointer parent = new Context {};
  Context child { parent };
  child.define("...", value);
  Value::OrError result = child.getValue("...");
  Tester::confirm(valuesEqual(result, value));
}

void testNestedContext3() {
  Value::Pointer value { new NumberValue { 89.0 } };
  Value::Pointer value2 { new NumberValue { 0.0 } };
  Value::Pointer value3 { new NumberValue { 1234567.8910 } };
  Value::Pointer value4 { new NumberValue { 5555.0 } };
  Context::Pointer root = new Context {
    Context::ValueMap {
      { "root_v1", value },
      { "root_v2", value2 }
    }
  };
  Context::Pointer layer2 = new Context { root };
  layer2->define("layer2_v3", value3);
  Context::Pointer layer3 = new Context { layer2 };
  layer3->define("layer3_v4", value4);

  Tester::confirm(valuesEqual(layer3->getValue("root_v1"), value));
  Tester::confirm(valuesEqual(layer2->getValue("root_v1"), value));
  Tester::confirm(valuesEqual(root->getValue("root_v1"), value));

  Tester::confirm(valuesEqual(layer3->getValue("root_v2"), value2));
  Tester::confirm(valuesEqual(layer2->getValue("root_v2"), value2));
  Tester::confirm(valuesEqual(root->getValue("root_v2"), value2));

  Tester::confirm(valuesEqual(layer3->getValue("layer2_v3"), value3));
  Tester::confirm(valuesEqual(layer2->getValue("layer2_v3"), value3));

  Tester::confirm(valuesEqual(layer3->getValue("layer3_v4"), value4));
}

void testIdentifierDefine() {
  Context context {};
  Value::Pointer value { new NumberValue { 3.14159265 } };
  std::shared_ptr<IdentifierValue> identifier { new IdentifierValue {
    Token { "xyzabc123", Token::Type::Identifier }
  } };
  context.define(identifier, value);
  Tester::confirm(valuesEqual(context.getValue("xyzabc123"), value));
}
