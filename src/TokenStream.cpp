// File: src/TokenStream.cpp
// Purpose: A TokenStream converts a string into a sequence of Tokens (i.e.
//  the smallest units of Fleet syntax). For more documentation, see
//  src/TokenStream.hpp

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

// This method returns a boolean indicating whether a given character is
//  considered to be whitespace in Fleet.
bool TokenStream::isblank(char c) {
  return c == ' ' || c == '\n' || c == '\t' || c == '\r' || c ==  '\f' ||
    c == '\v';
}

// This method queues the next Token from the code string into the private
//  variable nextToken. This method *must* put a non-null value into nextToken
//  unless the code string has been exhausted.
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
      queueNext(); // Since otherwise nothing is queued
    }
    else {
      nextToken = {takeLineBreak(), Token::Type::LineBreak};
    }
  }
  else if (std::isspace(firstChar)) {
    takeWhitespace();
    queueNext(); // Since nothing will be queued if whitespace is taken
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

// This method returns the next Token; however, it does not change the "current
//  Token" to the next Token. I.e., calling peek() many times will have the
//  same result if next() is not used.
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

// This method returns the next Token and changes the "current Token" to the
//  next Token after that.
Token TokenStream::next() {
  // Get the next token
  Token token = peek(); 
  // Reset nextToken so that a different token is obtained next time
  lastToken = nextToken;
  nextToken = {}; 
  return token;
}

// This method returns a boolean indicating whether the TokenStream is
//  exhausted (i.e. whether all Tokens have been extracted from the code
//  string).
bool TokenStream::hasNext() {
  if (!nextToken) {
    queueNext();
  }
  return !!nextToken;
}

// This method returns all whitespace at the beginning of the string and moves
//  the beginning of the string to after the whitespace. New lines are not
//  counted as whitespace for this method.
std::string TokenStream::takeWhitespace() {
  int originalIndex = index;
  char c;
  while (index < code.length()) {
    c = code.at(index);
    if (c == '\n' || !TokenStream::isblank(c)) {
      break;
    }
    index++;
  }
  return code.substr(originalIndex, index - originalIndex);
}

// This method returns all characters up to but not including a new line.
std::string TokenStream::takeComment() {
  int originalIndex = index;
  while (index < code.length() && code.at(index) != '\n') {
    index++;
  }
  return code.substr(originalIndex, index - originalIndex);
}

// This method returns the next character at the beginning of the string.
std::string TokenStream::takeGrouper() {
  if (index >= code.length()) {
    return "";
  }
  index++;
  return code.substr(index - 1, 1);
}

// This method returns the next identifier at the beginning of the string.
// An identifier is a sequence of alphanumeric characters or underscores. It
//  should not begin with a number; however, this condition is not checked
//  by this function.
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

// This method returns the next character at the beginning of the string.
std::string TokenStream::takeLineBreak() {
  if (index >= code.length()) {
    return "";
  }
  index++;
  return code.substr(index - 1, 1);
}

// This method returns the number at the beginning of the string. A number is
//  a sequence of 0-9 numerals with at most 1 . in the sequence as well.
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

// This method returns the operator at the beginning of the string. An operator
//  is a sequence of non-alphanumeric characters that are also not groupers or
//  whitespace or quotes.
std::string TokenStream::takeOperator() {
  int originalIndex = index;
  char c;
  while (index < code.length()) {
    c = code.at(index);
    if (TokenStream::isblank(c) || std::isalnum(c) || c == '#' || c == '_' ||
        c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' ||
        c == '"' || c == '\'') {
      break;
    }
    index++;
  }
  return code.substr(originalIndex, index - originalIndex);
}

// This method returns the string at the beginning of the code string being
//  interpreted. A string starts with a quote (either " or ') and ends with the
//  same character. Quotes can also be contained in a string if escaped by a
//  backslash.
std::string TokenStream::takeString() {
  if (index >= code.length()) {
    return "";
  }
  
  int originalIndex = index;
  char quoteType = code.at(index);
  index++;

  char c;
  bool lastWasQuote = false;
  while (index < code.length()) {
    c = code.at(index);
    if (c == '\\') {
      takeEscape();
    }
    else if (c == quoteType) {
      lastWasQuote = true;
      break;
    }
    index++;
  }
  index++;
  if (!lastWasQuote) {
    throw ParseError("Unclosed string");
  }
  return code.substr(originalIndex, index - originalIndex);
}

// This method returns the string starting with a backslash and containing
//  the next character as well (e.g. \n).
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
