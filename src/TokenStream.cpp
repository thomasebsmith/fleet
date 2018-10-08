#include <cctype>
#include <optional>
#include <string>
#include "ParseError.hpp"
#include "Token.hpp"
#include "TokenStream.hpp"

TokenStream::TokenStream(std::string codeString): code(codeString) {
  // All members are initialized in the initializer list above or in their
  // declarations.
}

// This method queues the next token from the code string into the private
// variable nextToken. This method *must* put a non-null value into nextToken
// unless the code string has been exhausted.
void TokenStream::queueNext() {
  if (nextToken) {
    return;
  }
  if (index >= code.length()) {
    return;
  }
  char firstChar = code.at(index);
  if (firstChar == '\n') {
    if (lastToken && lastToken->getType() == Token::Type::Operator) {
      takeLineBreak();
    }
    else {
      nextToken = {takeLineBreak(), Token::Type::LineBreak};
    }
  }
  else if (std::isspace(firstChar)) {
    takeWhitespace();
  }
  else if (std::isdigit(firstChar)) {
    nextToken = {takeNumber(), Token::Type::Number};
  }
  else if (std::isalpha(firstChar) || firstChar == '_') {
    nextToken = {takeIdentifier(), Token::Type::Identifier};
  }
  else if (firstChar == '#') {
    nextToken = {takeComment(), Token::Type::Comment};
  }
  else if (firstChar == '"' || firstChar == '\'') {
    nextToken = {takeString(), Token::Type::String};
  }
  else if (firstChar == '(' || firstChar == ')' || firstChar == '[' ||
      firstChar == ']' || firstChar == '{' || firstChar == '}') {
    nextToken = {takeGrouper(), Token::Type::Grouper};
  }
  else {
    nextToken = {takeOperator(), Token::Type::Operator};
  }
}

Token TokenStream::peek() {
  if (!nextToken) {
    queueNext();
  }

  // Assume that if no token has been added, we have reached the end of the
  // code string.
  if (!nextToken) { 
    throw std::out_of_range("Token stream is exhausted");
  }
  return *nextToken;
}

Token TokenStream::next() {
  // Get the next token
  Token token = peek(); 
  // Reset nextToken so that a different token is obtained next time
  lastToken = nextToken;
  nextToken = {}; 
  return token;
}

bool TokenStream::hasNext() {
  if (!nextToken) {
    queueNext();
  }
  return !!nextToken;
}

std::string TokenStream::takeWhitespace() {
  int originalIndex = index;
  char c;
  while (index < code.length()) {
    c = code.at(index);
    if (c == '\n' || !std::isblank(c)) {
      break;
    }
    index++;
  }
  return code.substr(originalIndex, index - originalIndex);
}

std::string TokenStream::takeComment() {
  int originalIndex = index;
  while (index < code.length() && code.at(index) != '\n') {
    index++;
  }
  return code.substr(originalIndex, index - originalIndex);
}

std::string TokenStream::takeGrouper() {
  index++;
  if (index < code.length()) {
    return code.substr(index - 1, 1);
  }
  return "";
}

std::string TokenStream::takeIdentifier() {
  int originalIndex = index;
  char c;
  while (index < code.length()) {
    c = code.at(index);
    if (!std::isalnum(c) && c != '_') {
      break;
    }
    index++;
  }
  return code.substr(originalIndex, index - originalIndex);
}

std::string TokenStream::takeLineBreak() {
  if (index >= code.length()) {
    return "";
  }
  index++;
  return code.substr(index - 1, 1);
}

std::string TokenStream::takeNumber() {
  int originalIndex = index;
  char c;
  bool dotFound = false;
  while (index < code.length()) {
    c = code.at(index);
    if (!dotFound && c == '.') {
      dotFound = true;
    }
    else if (!std::isdigit(code.at(index))) {
      break;
    }
    index++;
  }
  return code.substr(originalIndex, index - originalIndex);
}

std::string TokenStream::takeOperator() {
  int originalIndex = index;
  char c;
  while (index < code.length()) {
    c = code.at(index);
    if (std::isblank(c) || std::isalnum(c) || c == '#' || c == '_' ||
        c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' ||
        c == '"' || c == '\'') {
      break;
    }
    index++;
  }
  return code.substr(originalIndex, index - originalIndex);
}

std::string TokenStream::takeString() {
  if (index >= code.length()) {
    return "";
  }
  
  int originalIndex = index;
  char quoteType = code.at(index);
  index++;

  char c;
  while (index < code.length()) {
    c = code.at(index);
    if (c == '\\') {
      takeEscape();
    }
    else if (c == quoteType) {
      break;
    }
    index++;
  }
  index++;
  if (index >= code.length()) {
    throw ParseError("Unclosed string");
  }
  return code.substr(originalIndex, index - originalIndex);
}

std::string TokenStream::takeEscape() {
  if (index >= code.length()) {
    return "";
  }
  int originalIndex = index;
  index++; // Ignore the backslash
  if (index >= code.length()) {
    throw ParseError("Unfinished string escape");
  }
  index++; // Skip the second (escaped) character
  return code.substr(originalIndex, index - originalIndex);
}
