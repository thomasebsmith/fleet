// File: src/TokenTree.cpp
// Purpose: Source file for TokenTrees, which represent a structured tree of
//  Fleet Tokens. A TokenTree can be composed of one or more lines of
//  TokenTrees, a function and its argument, or a Token. Technically, TokenTrees
//  can also be "implied" - i.e. they represent implicit arguments to a function
//  such as the first argument in `(+ 3)`. For more documentation, see
//  src/TokenTree.hpp.

#include <memory>
#include <optional>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>
#include "ParseError.hpp"
#include "Token.hpp"
#include "TokenStream.hpp"
#include "TokenTree.hpp"
#include "TokenTreeVisitor.hpp"

// Constructors
TokenTree::TokenTree(const Token &value): data { value } {}

TokenTree::TokenTree(
  const TokenTree::TreePointer &f, const TokenTree::TreePointer &x
): data { TokenTree::FunctionPair {f, x} } {}

TokenTree::TokenTree(const TokenTree::LineList &lines):
  data { lines } {}

TokenTree::TokenTree(): data { std::monostate {} } {}

// The table of precedences for each operator. Precedences cannot be changed
//  during parsing.
// *Nothing* should have a precedence less than 0, since that is reserved for
//  function calls internally.
// Operators not specified in this table have a precedence of 60.
int TokenTree::defaultPrecedence = 60;
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

// The table of associativities for each operator. They default to being left-
//  associative (a value of `true`). Only `^` is right-associative right now.
std::unordered_map<std::string, bool> TokenTree::associativities {
  { "^", false }
};
bool TokenTree::defaultAssociativity = true; // Left-associative

// This function returns the precedence level of a given operator string.
int TokenTree::getPrecedence(std::string op) {
  if (TokenTree::precedences.count(op) > 0) {
    return TokenTree::precedences.at(op);
  }
  return TokenTree::defaultPrecedence;
}

// This function returns the associativity of a given operator string as a
//  boolean, where `true` represents left-associativity and `false` represents
//  right-associativity.
bool TokenTree::getAssociativity(std::string op) {
  if (TokenTree::associativities.count(op) > 0) {
    return TokenTree::associativities.at(op);
  }
  return TokenTree::defaultAssociativity;
}

// This function returns an optional that contains a Token iff this TokenTree
//  is a leaf (i.e. it contains only one Token).
std::optional<Token> TokenTree::getToken() const {
  if (std::holds_alternative<Token>(data)) {
    return { *std::get_if<Token>(&data) };
  }
  return {};
}

// This function returns an optional that contains a pair of TokenTrees iff
//  this TokenTree is a function call.
std::optional<
  std::pair<TokenTree, TokenTree>
> TokenTree::getFunctionPair() const {
  if (std::holds_alternative<TokenTree::FunctionPair>(data)) {
    const auto &pair = *std::get_if<TokenTree::FunctionPair>(&data);
    if (pair.first && pair.second) {
      std::pair<TokenTree, TokenTree> newPair { *pair.first, *pair.second };
      return { newPair };
    }
  }
  return {};
}

// This function returns an optional that contains a vector of TokenTrees iff
//  this TokenTree is a list of lines of code.
std::optional<std::vector<TokenTree>> TokenTree::getLineList() const {
  if (std::holds_alternative<TokenTree::LineList>(data)) {
    const auto &list = *std::get_if<TokenTree::LineList>(&data);
    std::vector<TokenTree> lines;
    unsigned int numLines = list.size();
    lines.reserve(numLines);
    for (unsigned int i = 0; i < numLines; i++) { 
      if (!list.at(i)) {
        return {};
      }
      lines.push_back(*list.at(i));
    }
    return { lines };
  }
  return {};
}

// This function returns a boolean indicating whether this TokenTree represents
//  an implied argument to a function.
bool TokenTree::isImplied() const {
  return std::holds_alternative<std::monostate>(data);
}

// This operator returns a boolean indicating whether this TokenTree is
//  equivalent to another TokenTree. To be true, both TokenTrees must be of the
//  same variant, and they must each contains values that equal each other.
bool TokenTree::operator==(const TokenTree &rhs) const {
  // Check to see if both are equivalent Tokens.
  const auto &leftToken = getToken();
  const auto &rightToken = rhs.getToken();
  if (leftToken || rightToken) {
    if (!leftToken || !rightToken) {
      return false;
    }
    return *leftToken == *rightToken;
  }

  // Check to see if both are equivalent function pairs.
  const auto &leftPair = getFunctionPair();
  const auto &rightPair = rhs.getFunctionPair();
  if (leftPair || rightPair) {
    if (!leftPair || !rightPair) {
      return false;
    }
    return *leftPair == *rightPair;
  }

  // Check to see if both are equivalent line lists.
  const auto &leftLines = getLineList();
  const auto &rightLines = rhs.getLineList();
  if (leftLines || rightLines) {
    if (!leftLines || !rightLines) {
      return false;
    }
    return *leftLines == *rightLines;
  }

  // Return true in all other cases (should only occur if both are implied).
  return true;
}

// This operator returns the string representation of a given TokenTree. The
//  string representation is formed as [Token], [f, x], or {line; line...}
//  depending on the variant of TokenTree.
TokenTree::operator std::string() const {
  // Tokens are represented as [Token].
  const auto &token = getToken();
  if (token) {
    return std::string("[") + static_cast<std::string>(*token) + "]";
  }

  // Function pairs are represented as [f, x].
  const auto &pair = getFunctionPair();
  if (pair) {
    return std::string("[") + static_cast<std::string>(pair->first) +
      ", " + static_cast<std::string>(pair->second) + "]";
  }

  // Line lists are represented as {lines...}.
  const auto &lines = getLineList();
  if (lines) {
    std::string result = "{";
    for (unsigned int i = 0; i < lines->size(); i++) {
      result += static_cast<std::string>(lines->at(i));
      if (i + 1 < lines->size()) {
        result += "; ";
      }
    }
    result += "}";
    return result;
  }

  // Implied arguments are represented as <implied>.
  return "<implied>";
}

// This function constructs a TokenTree from a given TokenStream. It uses a form
//  of the shunting-yard algorithm for operator precedence parsing modified to
//  parse Fleet-style function calls (i.e. function calls of the form `f x`).
TokenTree TokenTree::build(TokenStream stream) {
  std::stack<std::tuple<Token, int, bool>> operatorStack;
  std::stack<bool> lastWasNonOperatorStack;
  lastWasNonOperatorStack.push(false);

  TokenTree::LineList lines {};
  TokenTree::LineList outputQueue {};

  Token next;

  while (stream.hasNext()) {
    next = stream.next();
    switch (next.getType()) {
      case Token::Type::Identifier:
      case Token::Type::Number:
      case Token::Type::String:
        if (lastWasNonOperatorStack.top()) {
          // If the last stream Token was not an operator, then this is a
          //  function call, so create a TokenTree with the function and its
          //  argument.

          // The function should have already been pushed to the output queue.
          if (outputQueue.empty()) {
            throw ParseError("Internal parsing error");
          }
          const auto firstArg = outputQueue.back();
          TokenTree::TreePointer lastArg { new TokenTree { next } };
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
          // If the stream Token is (, [, or {, push the grouper onto the
          //  operator stack and enter a new layer of parsing.
          operatorStack.push({ next, 0, false });
          lastWasNonOperatorStack.push(false);
        }
        else {
          // If the stream Token is ), ], or }, apply operators from the
          //  operator stack until a matching grouper is found.
          Token t;
          bool grouperWasClosed = false;
          while (!operatorStack.empty()) {
            t = std::get<0>(operatorStack.top());
            operatorStack.pop();
            if (t.getType() == Token::Type::Grouper) {
              if (t.matchingGrouper() != next) {
                // If a non-matching grouper is found first, throw a parse
                //  error.
                throw ParseError("Unmatched " + t.getValue());
              }
              grouperWasClosed = true;
              break;
            }
            else {
              // Apply the operator to the output queue.
              // If there are no items in the output queue, simply push the
              //  operator onto the output queue.
              // If there is one item in the output queue and it is implied,
              //  push the operator onto output queue.
              // If there is one item in the output queue and it is not implied,
              //  push the operator with one argument applied onto the output
              //  queue.
              // Otherwise, push the operator with two arguments applied onto
              //  the output queue.
              if (outputQueue.empty()) {
                outputQueue.emplace_back(new TokenTree { t });
              }
              else if (outputQueue.size() == 1 &&
                       outputQueue.back()->isImplied()) {
                outputQueue.pop_back();
                outputQueue.emplace_back(new TokenTree { t });
              }
              else if (outputQueue.size() == 1) {
                const auto lastArg = outputQueue.back();
                TokenTree::TreePointer firstArg { new TokenTree { t }};
                outputQueue.pop_back();
                outputQueue.emplace_back(new TokenTree {
                  firstArg,
                  lastArg
                });
              }
              else {
                const auto lastArg = outputQueue.back();
                outputQueue.pop_back();
                const auto secondToLastArg = outputQueue.back();
                outputQueue.pop_back();
                TokenTree::TreePointer op { new TokenTree { t } };
                TokenTree::TreePointer firstFunc {
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
            if (outputQueue.empty()) {
              throw ParseError("Internal parsing error");
            }
            const auto next = outputQueue.back();
            outputQueue.pop_back();
            const auto secondToLast = outputQueue.back();
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
        // If a line break is encountered, evaluate all operators (see comments
        //  above for details) and add the result to the vector of lines.
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
            if (outputQueue.empty()) {
              outputQueue.emplace_back(new TokenTree { t });
            }
            else if (outputQueue.size() == 1 &&
                     outputQueue.back()->isImplied()) {
              outputQueue.pop_back();
              outputQueue.emplace_back(new TokenTree { t });
            }
            else if (outputQueue.size() == 1) {
              const auto lastArg = outputQueue.back();
              TokenTree::TreePointer firstArg { new TokenTree { t }};
              outputQueue.pop_back();
              outputQueue.emplace_back(new TokenTree {
                firstArg,
                lastArg
              });
            }
            else {
              const auto lastArg = outputQueue.back();
              outputQueue.pop_back();
              const auto secondToLastArg = outputQueue.back();
              outputQueue.pop_back();
              TokenTree::TreePointer op { new TokenTree { t } };
              TokenTree::TreePointer firstFunc { new TokenTree {
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
          const auto last = outputQueue.back();
          outputQueue.pop_back();
          lines.push_back(last);
        }
        break;
      case Token::Type::Operator: {
        // If an operator is encountered and the output queue is empty (i.e.
        //  the operator is the first token encountered in this grouping), then
        //  add an implied first argument.
        lastWasNonOperatorStack.top() = false;
        int precedence = getPrecedence(next.getValue());
        bool associativity = getAssociativity(next.getValue());
        if (outputQueue.empty()) {
          outputQueue.emplace_back(new TokenTree {
            // An implied argument
          });
        }

        // Apply operators until all Tokens are exhausted or until an operator
        //  with a lower precedence (or equal precedence if right-associative)
        //  is encountered.
        while (!operatorStack.empty() &&
          !std::get<0>(operatorStack.top()).isOpeningGrouper() && (
          std::get<1>(operatorStack.top()) > precedence ||
          (std::get<1>(operatorStack.top()) == precedence &&
            std::get<2>(operatorStack.top()) == true
          )
        )) {
          Token poppedOperator = std::get<0>(operatorStack.top());
          operatorStack.pop();
          if (outputQueue.empty()) {
            outputQueue.emplace_back(new TokenTree { poppedOperator });
          }
          else if (outputQueue.size() == 1 && outputQueue.back()->isImplied()) {
            outputQueue.pop_back();
            outputQueue.emplace_back(new TokenTree { poppedOperator });
          }
          else if (outputQueue.size() == 1) {
            const auto lastArg = outputQueue.back();
            TokenTree::TreePointer firstArg { new TokenTree {
              poppedOperator
            }};
            outputQueue.pop_back();
            outputQueue.emplace_back(new TokenTree {
              firstArg,
              lastArg
            });
          }
          else {
            const auto lastArg = outputQueue.back();
            outputQueue.pop_back();
            const auto secondToLastArg = outputQueue.back();
            outputQueue.pop_back();
            TokenTree::TreePointer poppedTree { new TokenTree {
              poppedOperator
            }};
            TokenTree::TreePointer firstFunc { new TokenTree {
              poppedTree, secondToLastArg
            }};
            outputQueue.emplace_back(new TokenTree { firstFunc,  lastArg });
          }
        }
        operatorStack.push({ next, precedence, associativity });
        }
        break;
      default:
        // Comments are ignored.
        continue;
    }
  }

  // Once all Tokens from the TokenStream are exhausted, empty the operator
  //  stack and output queue by applying operators and groupers (see above
  //  comments).
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
      else if (outputQueue.size() == 1 && outputQueue.back()->isImplied()) {
        outputQueue.pop_back();
        outputQueue.emplace_back(new TokenTree { t });
      }
      else if (outputQueue.size() == 1) {
        const auto lastArg = outputQueue.back();
        TokenTree::TreePointer firstArg { new TokenTree { t }};
        outputQueue.pop_back();
        outputQueue.emplace_back(new TokenTree {
          firstArg,
          lastArg
        });
      }
      else {
        const auto lastArg = outputQueue.back();
        outputQueue.pop_back();
        const auto secondToLastArg = outputQueue.back();
        outputQueue.pop_back();
        TokenTree::TreePointer op { new TokenTree { t } };
        TokenTree::TreePointer firstFunc { new TokenTree {
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

  // Return a TokenTree containing all the lines of TokenTrees that were
  //  created.
  return { lines };
}
