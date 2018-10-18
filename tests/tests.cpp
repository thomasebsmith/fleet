#include <iostream>
#include "TestToken.hpp"
#include "TestTokenStream.hpp"
#include "TestTokenTree.hpp"

int main() {
  int result =
    TestToken::main() + TestTokenStream::main() + TestTokenTree::main();
  std::cout << "\n\n";
  if (result == 0) {
    std::cout << "All tests PASSED!\n";
  }
  else {
    std::cout << "A total of " << result << " tests were FAILED!\n";
  }
}
