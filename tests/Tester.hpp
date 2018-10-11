#ifndef TESTER_HPP
#define TESTER_HPP

#include <string>
#include <utility>
#include <vector>

#ifndef TESTER_HPP_NOMACRO
#define confirm(condition) confirmCondition(condition, __LINE__)
#endif

class Tester {
private:
  int testsPassed;
  int testsFailed;
  std::string testerName;
  std::vector<std::pair<std::string, void (*)()>> tests;
public:
  Tester(std::string name);
  int getPassed();
  int getFailed();
  void test(std::string name, void (*testFunction)());
  int run();

  static void confirmCondition(bool condition, int line = 0);
};

#endif
