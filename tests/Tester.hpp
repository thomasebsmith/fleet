// File: tests/Tester.hpp
// Purpose: Header file for Testers, which allow a program to easily run a set
//  of tests, including with a useful assertion function and post-test metrics.

#ifndef TESTER_HPP
#define TESTER_HPP

#include <string>
#include <utility>
#include <vector>

// MACRO confirm(condition) - Asserts that `condition` is true. The macro is
//  needed to insert the current line number automatically.
//  To prevent this macro from being defined, define TESTER_HPP_NOMACRO.
#ifndef TESTER_HPP_NOMACRO
#define confirm(condition) confirmCondition((condition), (__LINE__))
#endif

class Tester {
private:
  int testsPassed;
  int testsFailed;
  std::string testerName;
  std::vector<std::pair<std::string, void (*)()>> tests;
public:
  // Constructor(name) - Creates a test set with the given name.
  Tester(std::string name);

  // getPassed() - Gets the number of tests passed. Should be used after run()
  //  is called.
  int getPassed();

  // getFailed() - Gets the number of tests failed. Should be used after run()
  //  is called.
  int getFailed();

  // test(name, testFunction) - Adds a test to the test set with the given name
  //  and the given execution function.
  void test(std::string name, void (*testFunction)());

  // run() - Runs all the tests, returning the number failed.
  int run();

  // static confirmCondition(condition, line) - Asserts that `condition` is
  //  true. If it is not, produces an error message with the line number
  //  indicated. Should most often be used within the confirm() macro.
  static void confirmCondition(bool condition, int line = 0);
};

#endif
