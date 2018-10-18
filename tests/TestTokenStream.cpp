#include <string>
#include "TestTokenStream.hpp"
#include "Tester.hpp"
#include "Token.hpp"
#include "TokenStream.hpp"

// Function declarations
void emptyCode();
void basicEndpointTokens();
void grouperTokens();
void allTokens();

int TestTokenStream::main() {
  Tester tester("Token Stream Tests");
  tester.test("Empty code", emptyCode);
  tester.test("Basic tokens", basicEndpointTokens);
  tester.test("Groupers", grouperTokens);
  tester.test("All tokens", allTokens);
  return tester.run();
}

void emptyCode() {
  TokenStream empty("");
  Tester::confirm(!empty.hasNext());
  TokenStream whitespace("  \t");
  Tester::confirm(!whitespace.hasNext());
  Tester::confirm(!empty.hasNext()); // Check again!
}

void basicEndpointTokens() {
  TokenStream basic {" foo0 _ba_r 1.3 \"this \\\"is a test\" "};

  Token toPeek {"foo0", Token::Type::Identifier };
  Tester::confirm(basic.peek() == toPeek);
  Tester::confirm(basic.next() == toPeek);
  
  Tester::confirm(basic.next() == (Token { "_ba_r", Token::Type::Identifier }));
  Tester::confirm(basic.next() == (Token { "1.3", Token::Type::Number }));
  Tester::confirm(basic.next() == (Token {
    "\"this \\\"is a test\"", Token::Type::String
  }));
  Tester::confirm(!basic.hasNext());
}

void grouperTokens() {
  TokenStream groupers {"(( [foo bar]\t{}{  )"};
  Tester::confirm(groupers.next() == (Token { "(", Token::Type::Grouper }));
  Tester::confirm(groupers.next() == (Token { "(", Token::Type::Grouper }));
  Tester::confirm(groupers.next() == (Token { "[", Token::Type::Grouper }));
  Tester::confirm(groupers.next() == (Token { "foo", Token::Type::Identifier }));
  Tester::confirm(groupers.next() == (Token { "bar", Token::Type::Identifier }));
  Tester::confirm(groupers.next() == (Token { "]", Token::Type::Grouper }));
  Tester::confirm(groupers.next() == (Token { "{", Token::Type::Grouper }));
  Tester::confirm(groupers.next() == (Token { "}", Token::Type::Grouper }));
  Tester::confirm(groupers.next() == (Token { "{", Token::Type::Grouper }));
  Tester::confirm(groupers.next() == (Token { ")", Token::Type::Grouper }));
  Tester::confirm(!groupers.hasNext());
}

void allTokens() {
  TokenStream all
    {"F x=5. 3\n\n4 +-**a!\n 7(){1} bar_0 'foobar \\'is'#foo#a\n"};
  Tester::confirm(all.next() == (Token { "F", Token::Type::Identifier }));
  Tester::confirm(all.next() == (Token { "x", Token::Type::Identifier }));
  Tester::confirm(all.next() == (Token { "=", Token::Type::Operator }));
  Tester::confirm(all.next() == (Token { "5.", Token::Type::Number }));
  Tester::confirm(all.next() == (Token { "3", Token::Type::Number }));
  Tester::confirm(all.next() == (Token { "\n", Token::Type::LineBreak }));
  Tester::confirm(all.next() == (Token { "\n", Token::Type::LineBreak }));
  Tester::confirm(all.next() == (Token { "4", Token::Type::Number }));
  Tester::confirm(all.next() == (Token { "+-**", Token::Type::Operator }));
  Tester::confirm(all.next() == (Token { "a", Token::Type::Identifier }));
  Tester::confirm(all.next() == (Token { "!", Token::Type::Operator }));
  Tester::confirm(all.next() == (Token { "7", Token::Type::Number }));
  Tester::confirm(all.next() == (Token { "(", Token::Type::Grouper }));
  Tester::confirm(all.next() == (Token { ")", Token::Type::Grouper }));
  Tester::confirm(all.next() == (Token { "{", Token::Type::Grouper }));
  Tester::confirm(all.next() == (Token { "1", Token::Type::Number }));
  Tester::confirm(all.next() == (Token { "}", Token::Type::Grouper }));
  Tester::confirm(all.next() == (Token { "bar_0", Token::Type::Identifier }));
  Tester::confirm(all.next() == (Token { "'foobar \\'is'", Token::Type::String }));
  Tester::confirm(all.next() == (Token { "#foo#a", Token::Type::Comment }));
  Tester::confirm(all.next() == (Token { "\n", Token::Type::LineBreak }));
  Tester::confirm(!all.hasNext());
}
