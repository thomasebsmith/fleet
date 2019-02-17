# Fleet
#### Documentation

## Overview
Fleet is a simple, functional programming language.
This project includes an interpreter written in C++.

Fleet (as a language) could also be compiled, but no compiler currently exists.

## Project Status
This project is a work-in-progress. Currently, only the tokenizer and abstract
syntax tree builder are implemented (with accompanying tests).

## Building the Project
##### You need:
 * `git`
 * `g++` with C++17 support
 * `make`
 * A Unix-compatible shell

##### Instructions:
 1. Clone the project:
  ```
  git clone https://github.com/thomasebsmith/fleet.git
  ```
 2. Navigate to the new folder:
  ```
  cd fleet
  ```
 3. Build the appropriate target:
  ```
  make tests
  ```
  or simply
  ```
  make
  ```
 4. Run the resulting executable:
  ```
  ./build/fleet
  ```
  or
  ```
  ./build/testFleet
  ```
  depending on what target was built.

Note: You can run `make debug` or `make debugtests` to build Fleet with
debugging hooks enabled.

## About the Language
Fleet's philosophy is one of simplicity: its grammar is extremely simple, with
no keywords or special cases. The language implements as little built-in
functionality as possible, relying on the standard library. The standard
library attempts to be consistent and universal.

Fleet is statically typed, but it offers type inference. This allows you to
produce clean code without sacrificing type safety. Fleet will also have modern
features like optionals, generics, and implementations of basic data structures
available in its standard library.

Fleet is based on a number of programming languages, including Haskell, Swift,
C++, and many more. Fleet as a language is suitable for virtually any
programming task. However, its current implementation is only available where
C++ can be compiled.

##### Grammar
```
comment: /#.*?\n/
grouper: /[()[\]{}]/
identifier: /[a-zA-Z_][a-zA-Z_0-9]*/
linebreak: \n when the last token was not an operator
number: /([0-9]+\.[0-9]*)|([0-9]*\.[0-9]+)/
operator: /[^a-zA-Z0-9_#\s]+/
string: /("|')([^\1]|(\\\1))\1/

grouping: grouper atom [the same grouper]
atom: identifier | grouping | string | number | functionPair | operation
functionPair: atom atom
operation: atom operator atom

```
Whitespace (other than new lines) is ignored. Comments are parsed and then
ignored.

Groupings have the highest precedence, followed by function pairs and then
by operations, which vary in precedence.
