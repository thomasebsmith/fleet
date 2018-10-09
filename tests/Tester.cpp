#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "Tester.hpp"

Tester::Tester(std::string name):
  testerName{name}, testsPassed{0}, testsFailed{0} {}
int Tester::getPassed() {
  return testsPassed;
}
int Tester::getFailed() {
  return testsFailed;
}
void Tester::test(std::string name, void (*testFunction)()) {
  tests.emplace_back(name, testFunction);
}
int Tester::run() {
  testsPassed = 0;
  testsFailed = 0;
  std::cout << "Running tests for " + testerName + "...\n===\n";
  for (size_t i = 0; i < tests.size(); i++) {
    try {
      tests.at(i).second();
      testsPassed++;
    }
    catch (...) {
      testsFailed++;
      std::cout << "FAILED test " + tests.at(i).first + "\n";
    }
  }
  std::cout << "\n===\n";
  std::cout << "Overall: Passed " << testsPassed << "/" << testsPassed +
    testsFailed << " tests.\n";
  if (testsFailed != 0) {
    std::cout <<
      "Tests were FAILED!! Check the messages above for more information.\n";
  }
  else {
    std::cout << "All tests PASSED!!\n";
  }
  return testsFailed;
}
void Tester::confirm(bool condition) {
  if (!condition) {
    throw std::runtime_error("Assertion failed");
  }
}
