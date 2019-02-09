// File: tests/TestToken.cpp
// Purpose: Source file for the TestToken test set.
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

// main() - Runs all Token tests.
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

// createToken() - Tests that Tokens can be created with basic string contents,
//  including empty strings.
void createToken() {
  Token myToken { "some normal value", Token::Type::Comment };
  Token anotherToken { "", Token::Type::String };
  Token finalToken { "!@FOO", Token::Type::Number };
}

// getValue() - Tests that the Token getValue() method correctly returns string
//  contents of different types of Tokens.
void getValue() {
  std::string value = "my value";
  std::string value2 = "";
  Token myToken { value, Token::Type::Comment };
  Token anotherToken { value2, Token::Type::Grouper };
  Tester::confirm(value == myToken.getValue());
  Tester::confirm(value2 == anotherToken.getValue());
}

// setValue() - Tests that the setValue() method works consistently with the
//  getValue() method.
void setValue() {
  std::string origValue = "something";
  std::string newValue = "\n";
  Token myToken { origValue, Token::Type::Operator };
  myToken.setValue(newValue);
  Tester::confirm(newValue == myToken.getValue());
}

// getType() - Tests that the getType() method correctly returns the type that
//  the Token was created as.
void getType() {
  Token::Type type = Token::Type::Identifier;
  Token::Type type2 = Token::Type::LineBreak;
  std::string value2 = "";
  Token myToken { "This", type };
  Token anotherToken { "is literally a test\n\t", type2 };
  Tester::confirm(type == myToken.getType());
  Tester::confirm(type2 == anotherToken.getType());
}

// setType() - Tests that the setType() method works consistently with the
//  getType() method.
void setType() {
  Token::Type origType = Token::Type::Grouper;
  Token::Type newType = Token::Type::String;
  Token myToken { "", origType };
  myToken.setType(newType);
  Tester::confirm(newType == myToken.getType());
}

// checkEquality() - Checks that the == method correctly checks Token types
//  and contents for equality.
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

// checkGrouper() - Checks that the isOpeningGrouper() method correctly checks
//  whether a given Grouper Token is "(", "[", or "{".
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

// matchGrouper() - Checks whether the matchingGropuer() method correctly
//  finds the closing Grouper for a given opening Grouper.
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
