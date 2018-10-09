#ifndef TESTER_HPP
#define TESTER_HPP

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
  void run();
};

#endif
