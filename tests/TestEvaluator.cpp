#include <string>
#include <variant>
#include "TestEvaluator.hpp"
#include "Context.hpp"
#include "Evaluator.hpp"
#include "NumberValue.hpp"
#include "Tester.hpp"
#include "TokenTree.hpp"
#include "Value.hpp"

// Function prototypes
bool evaluatesToNumber(const Evaluator &eval, std::string code, double num);
void testRawNumbers();
void testWhitespace();

int TestEvaluator::main() {
  Tester tester("Evaluator tests");
  tester.test("Test raw numbers", testRawNumbers);
  tester.test("Test whitespace", testWhitespace);
  return tester.run();
}

bool evaluatesToNumber(Evaluator &eval, std::string code, double num) {
  const auto result = eval.evaluate(TokenTree::build({ code }));
  if (!std::holds_alternative<Value::Pointer>(result)) {
    return false;
  }
  const auto evaledNum = (*std::get_if<Value::Pointer>(&result))->
    castValue<NumberValue>();
  if (!evaledNum) {
    return false;
  }
  return evaledNum->getRawNumber() == num;
}

void testRawNumbers() {
  Evaluator eval { new Context() };
  Tester::confirm(evaluatesToNumber(eval, "2", 2.0));
  Tester::confirm(evaluatesToNumber(eval, "57.488", 57.488));
}

void testWhitespace() {
  Evaluator eval { new Context() };
  Tester::confirm(evaluatesToNumber(eval, " \t8.8", 8.8));
  Tester::confirm(evaluatesToNumber(eval, "10.00000 \n \n\n\t ", 10.0));
  Tester::confirm(evaluatesToNumber(eval, "\f87.65\t\t\n  ", 87.65));
}
