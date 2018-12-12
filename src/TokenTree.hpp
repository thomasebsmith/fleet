// File: src/TokenTree.cpp
// Purpose: Header file for TokenTrees, which represent a structured tree of
//  Fleet Tokens. A TokenTree can be composed of one or more lines of
//  TokenTrees, a function and its argument, or a Token. Technically, TokenTrees
//  can also be "implied" - i.e. they represent implicit arguments to a function
//  such as the first argument in `(+ 3)`. For implementations, see
//  src/TokenTree.cpp.

#ifndef TOKENTREE_HPP
#define TOKENTREE_HPP

#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>
#include "ParseError.hpp"
#include "Token.hpp"
#include "TokenStream.hpp"
#include "TokenTreeVisitor.hpp"

class TokenTree {
public:

  // These typedefs can be used to refer to the various states of a TokenTree
  //  succinctly.
  typedef std::shared_ptr<TokenTree> TreePointer;
  typedef std::pair<TreePointer, TreePointer> FunctionPair;
  typedef std::vector<TreePointer> LineList;

private:
  
  static std::unordered_map<std::string, int> precedences;
  static int defaultPrecedence;

  static std::unordered_map<std::string, bool> associativities;
  static bool defaultAssociativity;

  // The std::monostate alternative represents an implied argument - i.e.
  //  one in constructions like (+ 2), where the first argument to (+) is not
  //  represented by a token.
  const std::variant<Token, FunctionPair, LineList, std::monostate> data;

public:

  // Constructor(value) - Constructs a TokenTree from a single given Token.
  TokenTree(const Token &value);
  
  // Constructor(f, x) - Constructs a TokenTree as a function call with function
  //  f and argument x.
  TokenTree(const TreePointer &f, const TreePointer &x);

  // Constructor(lines) - Constructs a TokenTree as a list of the given
  //  TokenTrees.
  TokenTree(const LineList &lines);

  // Constructor() - Constructs an implied argument TokenTree.
  TokenTree();

  // accept(v) - Following the visitor paradigm, calls the appropriate visit
  //  method on v based on the contents of this TokenTree. Returns the same
  //  type as the visit method on v.
  template <typename T>
  T accept(const TokenTreeVisitor<T> &v) const {
    const auto &token = getToken();
    if (token) {
      return v.visit(*token);
    }
    const auto &functionPair = getFunctionPair();
    if (functionPair) {
      return v.visit(functionPair->first, functionPair->second);
    }
    const auto &lineList = getLineList();
    if (lineList) {
      return v.visit(*lineList);
    }
    if (std::holds_alternative<std::monostate>(data)) {
      return v.visit();
    }
    throw ParseError { "Internal error: Unable to accept TokenTree visitor" };
  }

  // getToken() - Returns a Token iff this TokenTree contains a Token.
  std::optional<Token> getToken() const;

  // getFunctionPair() - Returns a pair {f, x} iff this TokenTree contains a
  //  function pair.
  std::optional<std::pair<TokenTree, TokenTree>> getFunctionPair() const;

  // getLineList() - Returns a vector of lines iff this TokenTree contains a
  //  number of lines.
  std::optional<std::vector<TokenTree>> getLineList() const;

  // isImplied() - Returns a boolean indicating whether this TokenTree is
  //  implied.
  bool isImplied() const;

  // ==rhs - Returns a boolean indicating whether this TokenTree is equivalent
  //  to rhs, which only occurs when both are the same variant and their
  //  contents are equivalent.
  bool operator==(const TokenTree &rhs) const;

  // std::string - Returns a string representation of this TokenTree.
  //  For a Token, this is [token_value].
  //  For a function pair, this is [f, x].
  //  For a line list, this is {line,line,...}.
  //  For an implied argument, this is <implied>.
  operator std::string() const;

  // static getPrecedence(op) - Returns the predence of an operator string op.
  //  This is based on hard-coded values for special operators, defaulting to 60
  //  for other operators.
  static int getPrecedence(std::string op);

  // static getAssociativity(op) - Returns the associativity of an operator
  //  string op. This defaults to true (left-associative). A return value of
  //  false represents right-associativity.
  static bool getAssociativity(std::string op);

  // static build(stream) - Builds a TokenTree form the given TokenStream. For
  //  details of this function's workings, see src/TokenTree.cpp.
  static TokenTree build(TokenStream stream);
};

#endif
