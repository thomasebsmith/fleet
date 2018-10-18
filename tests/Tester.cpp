#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "Tester.hpp"

Tester::Tester(std::string name):
  testsPassed{0}, testsFailed{0}, testerName{name} {}
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
  std::cout << "\nRunning " + testerName + "...\n";
  for (size_t i = 0; i < tests.size(); i++) {
    try {
      tests.at(i).second();
      testsPassed++;
    }
    catch (std::runtime_error e) {
      testsFailed++;
      std::cout << "FAILED test " + tests.at(i).first + " (" + e.what() + ")\n";
    }
    catch (...) {
      testsFailed++;
      std::cout << "FAILED test " + tests.at(i).first + "\n";
    }
  }
  std::cout << "===\n";
  std::cout << "Overall: Passed " << testsPassed << "/" << testsPassed +
    testsFailed << " tests.\n";
  if (testsFailed != 0) {
    std::cout <<
      "Tests were FAILED!! Check the messages above for more information.\n";
  }
  return testsFailed;
}
void Tester::confirmCondition(bool condition, int line) {
  if (!condition) {
    std::string error = "Assertion failed";
    if (line > 0) {
      error += " on line " + std::to_string(line);
    }
    throw std::runtime_error(error);
  }
}
