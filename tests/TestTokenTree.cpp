#include "Tester.hpp"
#include "TestTokenTree.hpp"
#include "Token.hpp"
#include "TokenStream.hpp"
#include "TokenTree.hpp"

// Function declarations
void createWithToken();
void getOperatorInfo();
void emptyTree();
void oneExpression();
void basicFunction();
void operations();

int TestTokenTree::main() {
  Tester tester { "Token Tree Tests" };
  tester.test("Create with one token", createWithToken);
  tester.test("Get operator precedence and associativity", getOperatorInfo);
  tester.test("Empty tree", emptyTree);
  tester.test("One expression", oneExpression);
  tester.test("A basic function", basicFunction);
  tester.test("Some operations", operations);
  return tester.run();
}

void createWithToken() {
  Token someNumber {"doesn't matter", Token::Type::Number};
  TokenTree myTree { someNumber };
}

void getOperatorInfo() {
  Tester::confirm(
    TokenTree::getPrecedence("@#$%^&^%$") > TokenTree::getPrecedence("=")
  );
  Tester::confirm(TokenTree::getAssociativity("@@"));
  Tester::confirm(TokenTree::getAssociativity("+"));
  Tester::confirm(!TokenTree::getAssociativity("^"));
  Tester::confirm(
    TokenTree::getPrecedence("*") > TokenTree::getPrecedence("-")
  );
}

void emptyTree() {
  TokenStream empty { "" };
  TokenTree tree = TokenTree::build(empty);
  const auto &lines = tree.getLineList();
  Tester::confirm(lines && lines->size() == 0);
  
  TokenStream notQuiteEmpty("   \n \n#foobar\n");
  const auto &notQuiteEmptyLines = tree.getLineList();
  Tester::confirm(notQuiteEmptyLines && notQuiteEmptyLines->size() == 0);
}

void oneExpression() {
  TokenStream oneToken { " f " };
  const auto &oneTokenLines = TokenTree::build(oneToken).getLineList();
  Tester::confirm(oneTokenLines && oneTokenLines->size() == 1);
  const auto &endpoint = oneTokenLines->at(0).getToken();
  Tester::confirm(endpoint &&
    *endpoint == (Token {"f", Token::Type::Identifier}));
}

void basicFunction() {
  TokenStream func { "G foo" };
  const auto &funcLines = TokenTree::build(func).getLineList();
  Tester::confirm(funcLines && funcLines->size() == 1);
  const auto &call = funcLines->at(0).getFunctionPair();
  Tester::confirm(!!call);
  const auto &funcName = call->first.getToken();
  Tester::confirm(!!funcName);
  Tester::confirm(*funcName == (Token {"G", Token::Type::Identifier}));
  const auto &argName = call->second.getToken();
  Tester::confirm(*argName == (Token {"foo", Token::Type::Identifier}));
  
  TokenStream twoFunc { "0 1 \"foobar(3 \" " };
  const auto &twoFuncLines = TokenTree::build(twoFunc).getLineList();
  Tester::confirm(twoFuncLines && twoFuncLines->size() == 1);
  const auto &call1 = twoFuncLines->at(0).getFunctionPair();
  Tester::confirm(!!call1);
  const auto &call2 = call1->first.getFunctionPair();
  Tester::confirm(!!call2);
  const auto &funcName1 = call2->first.getToken();
  Tester::confirm(!!funcName1);
  Tester::confirm(*funcName1 == (Token {"0", Token::Type::Number}));
  const auto &argName1 = call2->second.getToken();
  Tester::confirm(argName1 && *argName1 == (Token {"1", Token::Type::Number}));
  const auto &argName2 = call1->second.getToken();
  Tester::confirm(argName2 &&
    *argName2 == (Token {"\"foobar(3 \"", Token::Type::String}));
}

void operations() {
  TokenStream operations { "1.3*5-7/3" };
  const auto &opLines = TokenTree::build(operations);
  const auto comparisonTree = TokenTree (TokenTree::LineList {
    TokenTree::TreePointer { new TokenTree {
      TokenTree::TreePointer { new TokenTree {
        TokenTree::TreePointer { new TokenTree {
          Token { "-", Token::Type::Operator }
        } },
        TokenTree::TreePointer { new TokenTree {
          TokenTree::TreePointer { new TokenTree {
            TokenTree::TreePointer {
              new TokenTree {
                Token { "*", Token::Type::Operator }
              }
            },
            TokenTree::TreePointer {
              new TokenTree {
                Token { "1.3", Token::Type::Number }
              }
            }
          } },
          TokenTree::TreePointer { new TokenTree {
            Token { "5", Token::Type::Number }
          } }
        } },
      } },
      TokenTree::TreePointer { new TokenTree {
        TokenTree::TreePointer { new TokenTree {
          TokenTree::TreePointer {
            new TokenTree {
              Token { "/", Token::Type::Operator }
            }
          },
          TokenTree::TreePointer {
            new TokenTree {
              Token { "7", Token::Type::Number }
            }
          }
        } },
        TokenTree::TreePointer { new TokenTree {
          Token { "3", Token::Type::Number }
        } }
      } }
    } }
  });
  Tester::confirm(opLines == comparisonTree);
}
