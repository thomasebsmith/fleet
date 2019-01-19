#include <variant>
#include "TestEvaluator.hpp"
#include "Context.hpp"
#include "Evaluator.hpp"
#include "NumberValue.hpp"
#include "Tester.hpp"
#include "TokenTree.hpp"
#include "Value.hpp"

// Function prototypes
void testRawNumbers();

int TestEvaluator::main() {
  Tester tester("Evaluator tests");
  tester.test("Test raw numbers", testRawNumbers);
  return tester.run();
}

void testRawNumbers() {
  Evaluator eval { new Context() };
  TokenStream stream { "2" };
  TokenTree tree = TokenTree::build(stream);
  const auto result = eval.evaluate(tree);
  Tester::confirm(std::holds_alternative<Value::Pointer>(result));
  const auto num = (*std::get_if<Value::Pointer>(&result))->
    castValue<NumberValue>();
  Tester::confirm(num->getRawNumber() == 2.0);
}
