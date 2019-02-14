// File: tests/TestEvaluator.cpp
// Purpose: Source file for the TestEvaluator test set.

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
void testMultiplication();
void testExponentiation();
void testCombinedOperations();
void testCombinedParens();

// main() - Runs all tests
int TestEvaluator::main() {
  Tester tester("Evaluator tests");
  tester.test("Test raw numbers", testRawNumbers);
  tester.test("Test whitespace", testWhitespace);
  tester.test("Test addition", testAddition);
  tester.test("Test multiplication", testMultiplication);
  tester.test("Test exponentiation", testExponentiation);
  tester.test("Test combined operations", testCombinedOperations);
  tester.test("Test operations and parentheses", testCombinedParens);
  return tester.run();
}

// evaluatesApproxTo(eval, code, num) - Returns a boolean indicating whether
//  the string `code`, when evaluated in `eval`, evaluates to a NumberValue
//  that is very close in precision to `num`.
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

// testRawNumbers() - Tests that raw number strings evaluate to their respective
//  doubles.
void testRawNumbers() {
  Evaluator eval { new Context() };
  Tester::confirm(evaluatesApproxTo(eval, "2", 2.0));
  Tester::confirm(evaluatesApproxTo(eval, "57.488", 57.488));
}

// testWhitespace() - Tests whether raw number strings with additional
//  whitespace evaluate correctly.
void testWhitespace() {
  Evaluator eval { new Context() };
  Tester::confirm(evaluatesApproxTo(eval, " \t8.8", 8.8));
  Tester::confirm(evaluatesApproxTo(eval, "10.00000 \n \n\n\t ", 10.0));
  Tester::confirm(evaluatesApproxTo(eval, "\f87.65\t\t\n  ", 87.65));
}

// testAddition() - Tests that addition of numbers works as expected.
void testAddition() {
  Evaluator eval { new DefaultContext() };
  Tester::confirm(evaluatesApproxTo(eval, "1.0 + 2.0", 3.0));
  Tester::confirm(evaluatesApproxTo(eval, "0+5", 5.0));
  Tester::confirm(evaluatesApproxTo(eval, "9.88+ 0.13", 10.01));
  Tester::confirm(evaluatesApproxTo(eval, "0.0001 +99.7", 99.7001));
}

// testMultiplication() - Tests that multiplication of numbers works as
//  expected.
void testMultiplication() {
  Evaluator eval { new DefaultContext() };
  Tester::confirm(evaluatesApproxTo(eval, "1 * 3.5", 3.5));
  Tester::confirm(evaluatesApproxTo(eval, "8 * 8", 64));
  Tester::confirm(evaluatesApproxTo(eval, "0.85 * 11", 9.35));
  Tester::confirm(evaluatesApproxTo(eval, "0.999 * 1.15", 1.14885));
}

// testExponentiation() - Tests whether exponentiation (raising numbers to
//  powers) works as expected.
void testExponentiation() {
  Evaluator eval { new DefaultContext() };
  Tester::confirm(evaluatesApproxTo(eval, "1 ^ 99.7", 1.0));
  Tester::confirm(evaluatesApproxTo(eval, "3.66 ^ 1", 3.66));
  Tester::confirm(evaluatesApproxTo(eval, "8.75 ^ 0.223", 1.622063290846));
  Tester::confirm(evaluatesApproxTo(eval, "3 ^ 11", 177147.0));
}

// testCombinedOperations() - Tests whether different operations evaluate
//  correctly when combined, including whether order of operations is respected.
void testCombinedOperations() {
  Evaluator eval { new DefaultContext() };
  Tester::confirm(evaluatesApproxTo(eval, "1 * 3 + 5", 8.0));
  Tester::confirm(evaluatesApproxTo(eval, "1 + 3 * 5", 16.0));
  Tester::confirm(evaluatesApproxTo(eval, "2.2 ^ 3.3 * 4.4 + 5.5", 64.8536626));
  Tester::confirm(evaluatesApproxTo(eval, "87.6 + 55.3 * 0.0", 87.6));
}

// testCombinedParens() - Tests whether different operations and groupings
//  evaluate correctly when combined.
void testCombinedParens() {
  Evaluator eval { new DefaultContext() };
  Tester::confirm(evaluatesApproxTo(eval, "3.1 ^ (1.1^(2*3)) + 4*(3+2)",
    27.42125259322668));
  Tester::confirm(evaluatesApproxTo(eval, "((((((5.9999))))))", 5.9999));
  Tester::confirm(evaluatesApproxTo(eval, "3+((5*7)+3)^2.2^   0.0", 41.0));
  Tester::confirm(evaluatesApproxTo(eval, "1^1^1^2^3*(5+1.1)^(2^0.01)",
    6.1772081531526535));
}
