#include <memory>
#include <optional>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
#include "ParseError.hpp"
#include "Token.hpp"
#include "TokenStream.hpp"
#include "TokenTree.hpp"

TokenTree::TokenTree(Token value): endpoint{value}, type{Type::Endpoint} {}
TokenTree::TokenTree(
  std::shared_ptr<TokenTree> f, std::shared_ptr<TokenTree> x
): functionCall{f, x}, type{Type::FunctionCall} {}
TokenTree::TokenTree(std::vector<std::shared_ptr<TokenTree> > lines):
  multiLine{lines}, type{Type::MultiLine} {}
TokenTree::~TokenTree() {
  switch (type) {
    case Type::Endpoint:
      endpoint.Token::~Token();
      break;
    case Type::FunctionCall:
      functionCall.~pair<
        std::shared_ptr<TokenTree>, std::shared_ptr<TokenTree>
      >();
      break;
    case Type::MultiLine:
      multiLine.~vector<std::shared_ptr<TokenTree>>();
      break;
    default:
      // Do nothing
      break;
  }
}

// *Nothing* should have a precedence less than 0, since that is reserved for
// function calls internally.
int TokenTree::defaultPrecedence = 50;
std::unordered_map<std::string, int> TokenTree::precedences = {
  { ".", 100},
  { ":", 90 },
  { "^", 80 },
  { "*", 70 },
  { "/", 70 },
  { "%", 70 },
  // Default precedence = 60
  { "+", 50 },
  { "++",50 },
  { "-", 50 },
  { "&", 40 },
  { "|", 40 },
  { "$", 30 },
  { ",", 20 },
  { ";", 10 },
  { "=", 0  }
};

// Default associativity = true (left-associative)
// false = right-associative
std::unordered_map<std::string, bool> TokenTree::associativities {
  { "^", false }
};
bool TokenTree::defaultAssociativity = true;

int TokenTree::getPrecedence(std::string op) {
  if (TokenTree::precedences.count(op) > 0) {
    return TokenTree::precedences.at(op);
  }
  return TokenTree::defaultPrecedence;
}

bool TokenTree::getAssociativity(std::string op) {
  if (TokenTree::associativities.count(op) > 0) {
    return TokenTree::associativities.at(op);
  }
  return TokenTree::defaultAssociativity;
}

TokenTree TokenTree::build(TokenStream stream) {
  std::stack<std::tuple<Token, int, bool>> operatorStack;
  std::stack<bool> lastWasNonOperatorStack;
  lastWasNonOperatorStack.push(false);

  std::vector<std::shared_ptr<TokenTree>> lines {};
  std::vector<std::shared_ptr<TokenTree>> outputQueue {};

  Token next;

  while (stream.hasNext()) {
    next = stream.next();
    switch (next.getType()) {
      case Token::Type::Identifier:
      case Token::Type::Number:
      case Token::Type::String:
        if (lastWasNonOperatorStack.top()) {
          if (outputQueue.size() == 0) {
            throw ParseError("Internal parsing error");
          }
          auto firstArg = outputQueue.back();
          std::shared_ptr<TokenTree> lastArg { new TokenTree { next } };
          outputQueue.pop_back();
          outputQueue.emplace_back(new TokenTree {
            firstArg, lastArg
          });
        }
        else {
          lastWasNonOperatorStack.top() = true;
          outputQueue.emplace_back(new TokenTree { next });
        }
        break;
      case Token::Type::Grouper:
        if (next.isOpeningGrouper()) {
          operatorStack.push({ next, 0, false });
          lastWasNonOperatorStack.push(false);
        }
        else {
          Token t;
          bool grouperWasClosed = false;
          while (!operatorStack.empty()) {
            t = std::get<0>(operatorStack.top());
            operatorStack.pop();
            if (t.getType() == Token::Type::Grouper) {
              if (t.matchingGrouper() != next) {
                throw ParseError("Unmatched " + t.getValue());
              }
              grouperWasClosed = true;
              break;
            }
            else {
              if (outputQueue.size() == 0) {
                outputQueue.emplace_back(new TokenTree { t });
              }
              else if (outputQueue.size() == 1) {
                auto lastArg = outputQueue.back();
                std::shared_ptr<TokenTree> firstArg { new TokenTree { t }};
                outputQueue.pop_back();
                outputQueue.emplace_back(new TokenTree {
                  firstArg,
                  lastArg
                });
              }
              else {
                auto lastArg = outputQueue.back();
                outputQueue.pop_back();
                auto secondToLastArg = outputQueue.back();
                outputQueue.pop_back();
                std::shared_ptr<TokenTree> op { new TokenTree { t } };
                std::shared_ptr<TokenTree> firstFunc {
                  new TokenTree { op, secondToLastArg }
                };
                outputQueue.emplace_back( new TokenTree {
                  firstFunc,
                  lastArg
                });
              }
            }
          }
          if (!grouperWasClosed) {
            throw ParseError("Unmatched " + next.getValue());
          }
          lastWasNonOperatorStack.pop();
          if (lastWasNonOperatorStack.top()) {
            if (outputQueue.size() == 0) {
              throw ParseError("Internal parsing error");
            }
            auto next = outputQueue.back();
            outputQueue.pop_back();
            auto secondToLast = outputQueue.back();
            outputQueue.pop_back();
            outputQueue.emplace_back(new TokenTree {
              secondToLast, next
            });
          }
          else {
            lastWasNonOperatorStack.top() = true;
          }
        }
        break;
      case Token::Type::LineBreak:
        while (!lastWasNonOperatorStack.empty()) {
          lastWasNonOperatorStack.pop();
        }
        lastWasNonOperatorStack.push(false);
        if (!outputQueue.empty() || !operatorStack.empty()) {
          Token t;
          while (!operatorStack.empty()) {
            t = std::get<0>(operatorStack.top());
            operatorStack.pop();
            if (t.getType() == Token::Type::Grouper) {
              throw ParseError("Unmatched " + t.getValue());
            }
            if (outputQueue.size() == 0) {
              outputQueue.emplace_back(new TokenTree { t });
            }
            else if (outputQueue.size() == 1) {
              auto lastArg = outputQueue.back();
              std::shared_ptr<TokenTree> firstArg { new TokenTree { t }};
              outputQueue.pop_back();
              outputQueue.emplace_back(new TokenTree {
                firstArg,
                lastArg
              });
            }
            else {
              auto lastArg = outputQueue.back();
              outputQueue.pop_back();
              auto secondToLastArg = outputQueue.back();
              outputQueue.pop_back();
              std::shared_ptr<TokenTree> op { new TokenTree { t } };
              std::shared_ptr<TokenTree> firstFunc { new TokenTree {
                op, secondToLastArg
              }};
              outputQueue.emplace_back(new TokenTree {
                firstFunc, lastArg
              });
            }
          }
          if (outputQueue.size() != 1) {
            throw ParseError("Internal parse error: output queue not empty");
          }
          lines.push_back(outputQueue.back());
          outputQueue.pop_back();
        }
        break;
      case Token::Type::Operator: {
        lastWasNonOperatorStack.top() = false;
        int precedence = getPrecedence(next.getValue());
        bool associativity = getAssociativity(next.getValue());
        while (!operatorStack.empty() &&
          !std::get<0>(operatorStack.top()).isOpeningGrouper() && (
          std::get<1>(operatorStack.top()) > precedence ||
          (std::get<1>(operatorStack.top()) == precedence &&
            std::get<2>(operatorStack.top()) == true
          )
        )) {
          Token poppedOperator = std::get<0>(operatorStack.top());
          operatorStack.pop();
          if (outputQueue.size() == 0) {
            outputQueue.emplace_back(new TokenTree { poppedOperator });
          }
          else if (outputQueue.size() == 1) {
            auto lastArg = outputQueue.back();
            std::shared_ptr<TokenTree> firstArg { new TokenTree {
              poppedOperator
            }};
            outputQueue.pop_back();
            outputQueue.emplace_back(new TokenTree {
              firstArg,
              lastArg
            });
          }
          else {
            auto lastArg = outputQueue.back();
            outputQueue.pop_back();
            auto secondToLastArg = outputQueue.back();
            outputQueue.pop_back();
            std::shared_ptr<TokenTree> poppedTree { new TokenTree {
              poppedOperator
            }};
            std::shared_ptr<TokenTree> firstFunc { new TokenTree {
              poppedTree, secondToLastArg
            }};
            outputQueue.emplace_back(new TokenTree { firstFunc,  lastArg });
          }
        }
        operatorStack.push({ next, precedence, associativity });
        }
        break;
      default:
        // Comments are ignored
        continue;
    }
  }
  if (!outputQueue.empty() || !operatorStack.empty()) {
    Token t;
    while (!operatorStack.empty()) {
      t = std::get<0>(operatorStack.top());
      operatorStack.pop();
      if (t.getType() == Token::Type::Grouper) {
        throw ParseError("Unmatched " + t.getValue());
      }
      if (outputQueue.size() == 0) {
        outputQueue.emplace_back(new TokenTree { t });
      }
      else if (outputQueue.size() == 1) {
        auto lastArg = outputQueue.back();
        std::shared_ptr<TokenTree> firstArg { new TokenTree { t }};
        outputQueue.pop_back();
        outputQueue.emplace_back(new TokenTree {
          firstArg,
          lastArg
        });
      }
      else {
        auto lastArg = outputQueue.back();
        outputQueue.pop_back();
        auto secondToLastArg = outputQueue.back();
        outputQueue.pop_back();
        std::shared_ptr<TokenTree> op { new TokenTree { t } };
        std::shared_ptr<TokenTree> firstFunc { new TokenTree {
          op, secondToLastArg
        }};
        outputQueue.emplace_back(new TokenTree { firstFunc, lastArg });
      }
    }
    if (outputQueue.size() != 1) {
      throw ParseError("Internal parse error: output queue not empty");
    }
    lines.push_back({outputQueue.back()});
    outputQueue.pop_back();
  }
  return { lines };
}
