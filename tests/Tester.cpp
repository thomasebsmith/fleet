// File: tests/Tester.cpp
// Purpose: Source file for Testers, which comprise a set of related tests and
//  allow for smart assertions within those tests.
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "Tester.hpp"

// constructor(name) - Creates a Tester with 0 initial tests and a given name.
Tester::Tester(std::string name):
  testsPassed{0}, testsFailed{0}, testerName{name} {}

// getPassed() - Returns the number of passed tests.
int Tester::getPassed() {
  return testsPassed;
}

// getFailed() - Returns the number of failed tests.
int Tester::getFailed() {
  return testsFailed;
}

// test(name, testFunction) - Adds a test with a given name and a given
//  execution function.
void Tester::test(std::string name, void (*testFunction)()) {
  tests.emplace_back(name, testFunction);
}

// run() - Runs all tests, counting the numbers passed and failed. Outputs the
//  results to standard output.
int Tester::run() {
  testsPassed = 0;
  testsFailed = 0;
  std::cout << "\nRunning " + testerName + "...\n";
  for (size_t i = 0; i < tests.size(); i++) {
    try {
      tests.at(i).second();
      testsPassed++;
    }
    catch (std::runtime_error e) {
      // If there is a runtime error, the test fails.
      testsFailed++;
      std::cout << "FAILED test " + tests.at(i).first + " (" + e.what() + ")\n";
    }
    catch (...) {
      // If there is any other type of error, the test fails.
      testsFailed++;
      std::cout << "FAILED test " + tests.at(i).first + "\n";
    }
  }
  // Print a summary with the number of tests passed and failed.
  std::cout << "===\n";
  std::cout << "Overall: Passed " << testsPassed << "/" << testsPassed +
    testsFailed << " tests.\n";
  if (testsFailed != 0) {
    std::cout <<
      "Tests were FAILED!! Check the messages above for more information.\n";
  }
  return testsFailed;
}

// confirmCondition(condition, line) - Asserts that `condition` is true (the
//  assertion should be on line `line`. Should be used as a macro (see
//  tests/Tester.hpp).
void Tester::confirmCondition(bool condition, int line) {
  if (!condition) {
    // If the condition is false, create an error message with the ilne number
    //  and the fact that the assertion failed.
    std::string error = "Assertion failed";
    if (line > 0) {
      error += " on line " + std::to_string(line);
    }
    throw std::runtime_error(error);
  }
}
