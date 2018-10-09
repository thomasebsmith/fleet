#include "Token.hpp"
#include "TokenTree.hpp"
#include "Tester.hpp"
#include "TestTokenTree.hpp"

// Function declarations
void createWithToken();

int TestTokenTree::main() {
  Tester tester{"Token Tree Tests"};
  tester.test("Create with one token", createWithToken);
  return tester.run();
}

int main() {
  return TestTokenTree::main();
}

void createWithToken() {
  Token someNumber("doesn't matter", Token::Type::Number);
  TokenTree myTree(someNumber);
}
