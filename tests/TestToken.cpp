#include <string>
#include "Tester.hpp"
#include "TestToken.hpp"
#include "Token.hpp"

// Function declarations
void createToken();
void getValue();
void setValue();
void getType();
void setType();
void checkEquality();
void checkGrouper();
void matchGrouper();

int TestToken::main() {
  Tester tester("Token tests");
  tester.test("Create a token", createToken);
  tester.test("Get a token's value", getValue);
  tester.test("Set a token's value", setValue);
  tester.test("Get a token's type", getType);
  tester.test("Set a token's type", setType);
  tester.test("Check tokens' equality and inequality", checkEquality);
  tester.test("Check whether a token is an opening grouper", checkGrouper);
  tester.test("Get a token's matching grouper", matchGrouper);
  return tester.run();
}

void createToken() {
  Token myToken { "some normal value", Token::Type::Comment };
  Token anotherToken { "", Token::Type::String };
  Token finalToken { "!@FOO", Token::Type::Number };
}
void getValue() {
  std::string value = "my value";
  std::string value2 = "";
  Token myToken { value, Token::Type::Comment };
  Token anotherToken { value2, Token::Type::Grouper };
  Tester::confirm(value == myToken.getValue());
  Tester::confirm(value2 == anotherToken.getValue());
}
void setValue() {
  std::string origValue = "something";
  std::string newValue = "\n";
  Token myToken { origValue, Token::Type::Operator };
  myToken.setValue(newValue);
  Tester::confirm(newValue == myToken.getValue());
}
void getType() {
  Token::Type type = Token::Type::Identifier;
  Token::Type type2 = Token::Type::LineBreak;
  std::string value2 = "";
  Token myToken { "This", type };
  Token anotherToken { "is literally a test\n\t", type2 };
  Tester::confirm(type == myToken.getType());
  Tester::confirm(type2 == anotherToken.getType());
}
void setType() {
  Token::Type origType = Token::Type::Grouper;
  Token::Type newType = Token::Type::String;
  Token myToken { "", origType };
  myToken.setType(newType);
  Tester::confirm(newType == myToken.getType());
}
void checkEquality() {
  Token t1 { "Foobar", Token::Type::Comment };
  Token t2 { "Foobar", Token::Type::LineBreak };
  Token t3 { "Foobar", Token::Type::Comment };
  Token t4 { "barfoo", Token::Type::LineBreak };
  Token t5 { "", Token::Type::Comment };
  Tester::confirm(!(t1 == t2));
  Tester::confirm(t1 != t2);
  Tester::confirm(t3 == t1);
  Tester::confirm(!(t3 != t1));
  Tester::confirm(!(t4 == t1));
  Tester::confirm(t4 != t1);
  Tester::confirm(!(t1 == t5));
  Tester::confirm(t1 != t5);
  Tester::confirm(t5 == t5);
  Tester::confirm(!(t5 != t5));
}
void checkGrouper() {
  Token t1 { "]", Token::Type::Grouper };
  Token t2 { "}", Token::Type::Operator };
  Token t3 { "", Token::Type::Grouper };
  Token t4 { "foo", Token::Type::String };
  Token t5 { "(", Token::Type::Grouper };
  Token t6 { "[", Token::Type::Grouper };
  Token t7 { "{", Token::Type::Grouper };
  Tester::confirm(!t1.isOpeningGrouper());
  Tester::confirm(!t2.isOpeningGrouper());
  Tester::confirm(!t3.isOpeningGrouper());
  Tester::confirm(!t4.isOpeningGrouper());
  Tester::confirm(t5.isOpeningGrouper());
  Tester::confirm(t6.isOpeningGrouper());
  Tester::confirm(t7.isOpeningGrouper());
}
void matchGrouper() {
  Token t1 { "(", Token::Type::Grouper };
  Token t2 { "[", Token::Type::Grouper };
  Token t3 { "{", Token::Type::Grouper };
  Token t4 { ")", Token::Type::Grouper };
  Token t5 { "]", Token::Type::Grouper };
  Token t6 { "}", Token::Type::Grouper };
  Tester::confirm(t1.matchingGrouper() == t4);
  Tester::confirm(t2.matchingGrouper() == t5);
  Tester::confirm(t3.matchingGrouper() == t6);
}
