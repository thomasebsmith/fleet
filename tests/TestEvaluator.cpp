#include <cmath>
#include <string>
#include <variant>
#include "TestEvaluator.hpp"
#include "Context.hpp"
#include "DefaultContext.hpp"
#include "Evaluator.hpp"
#include "NumberValue.hpp"
#include "Tester.hpp"
#include "TokenTree.hpp"
#include "Value.hpp"

// Function prototypes
bool evaluatesApproxTo(const Evaluator &eval, std::string code, double num);
void testRawNumbers();
void testWhitespace();
void testAddition();

int TestEvaluator::main() {
  Tester tester("Evaluator tests");
  tester.test("Test raw numbers", testRawNumbers);
  tester.test("Test whitespace", testWhitespace);
  tester.test("Test addition", testAddition);
  return tester.run();
}

bool evaluatesApproxTo(Evaluator &eval, std::string code, double num) {
  const double epsilon = 0.000001;
  const auto result = eval.evaluate(TokenTree::build({ code }));
  if (!std::holds_alternative<Value::Pointer>(result)) {
    return false;
  }
  const auto evaledNum = (*std::get_if<Value::Pointer>(&result))->
    castValue<NumberValue>();
  if (!evaledNum) {
    return false;
  }
  return std::abs(evaledNum->getRawNumber() - num) <= epsilon;
}

void testRawNumbers() {
  Evaluator eval { new Context() };
  Tester::confirm(evaluatesApproxTo(eval, "2", 2.0));
  Tester::confirm(evaluatesApproxTo(eval, "57.488", 57.488));
}

void testWhitespace() {
  Evaluator eval { new Context() };
  Tester::confirm(evaluatesApproxTo(eval, " \t8.8", 8.8));
  Tester::confirm(evaluatesApproxTo(eval, "10.00000 \n \n\n\t ", 10.0));
  Tester::confirm(evaluatesApproxTo(eval, "\f87.65\t\t\n  ", 87.65));
}

void testAddition() {
  Evaluator eval { new DefaultContext() };
  Tester::confirm(evaluatesApproxTo(eval, "1.0 + 2.0", 3.0));
  Tester::confirm(evaluatesApproxTo(eval, "0+5", 5.0));
  Tester::confirm(evaluatesApproxTo(eval, "9.88+ 0.13", 10.01));
  Tester::confirm(evaluatesApproxTo(eval, "0.0001 +99.7", 99.7001));
}
