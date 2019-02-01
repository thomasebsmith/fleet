// File: tests/tests.cpp
// Purpose: Source file for the main test function, which runs all tests and
//  outputs a summary of the results.
#include <iostream>
#include "TestContext.hpp"
#include "TestEvaluator.hpp"
#include "TestToken.hpp"
#include "TestTokenStream.hpp"
#include "TestTokenTree.hpp"

// main() - Runs all the tests specified in the code below. Adds up the number
//  of failed tests, and returns that number. If all tests passed, it outputs
//  that fact. If some tests failed, it outputs the number of tests failed.
int main() {
  int result =
    TestToken::main() + TestTokenStream::main() + TestTokenTree::main() +
    TestEvaluator::main() + TestContext::main();
  std::cout << "\n\n";
  if (result == 0) {
    std::cout << "All tests PASSED!\n";
  }
  else {
    std::cout << "A total of " << result << " tests were FAILED!\n";
  }
  return result;
}
