# Fleet Documentation

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
 * `g++` or `clang` with C++17 support
 * `make`
 * A Unix-compatible shell

##### Instructions:
 1. Clone the project:
  ```sh
  git clone https://github.com/thomasebsmith/fleet.git
  ```
 2. Navigate to the new folder:
  ```sh
  cd fleet
  ```
 3. Build the appropriate target:
  ```sh
  make
  ```
  or
  ```sh
  make tests
  ```
 4. Run the resulting executable:
  ```sh
  ./build/fleet
  ```
  or
  ```sh
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

##### Goals
Fleet has been designed with six main goals in mind:
 1. Syntactic simplicity.
 2. Types as arbitrary analysis-time constraints.
 3. Types as first-class values.
 4. Inferred static typing.
 5. No runtime errors.
 6. High performance.

##### Core Types
Numbers, characters, and functions are built into Fleet itself. These three
types form the basis of every other type. In the standard library, strings
are created as lists of characters. Note that characters are defined by Unicode
code points rather than UTF-8 or ASCII bytes.

No function in Fleet should throw an error unless that error is a type mismatch
or a parse error. Instead, functions that only sometimes return a valid value
should return an optional - a value that may or may not contain another value.

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

##### Prelude
Here is an incomplete list of prelude variables:
 - `import`
 - `Module`
 - `providing`
 - `scope`
 - `Any`
 - `Char`
 - `Float` (includes `Float32`, `Float64`, and `Float128`)
 - `Int` (includes `Byte`, `Int16`, `Int32`, `Int64`, and `Int128`)
 - `List`
 - `Num` (includes `Float` and `Int`)
 - `String` (an alias for `List Char`)
 - `Maybe T` (includes `Nothing` and `Some T`)
 - `=`
 - `+`
 - `-`
 - `*`
 - `/`
 - `^`
 - `&`
 - `|`
 - `::`
 - `[]`
 - `,`
 - `->`
 - `=>`
 - `.`

##### Standard Library
Fleet's standard library will contain many functions and types that can be
used in your code. Only a small portion of the standard library is included
in your code be default (this portion is called the prelude).

To include part of the standard library in your code, use this syntax:
```
Module = import "Module"
```

To prevent the program from including the prelude, use this syntax:
```
#[directive no_prelude]
```

To include a custom module in your code, use this syntax:
```
Module = import "./path/to/Module"
```

To import a module into the current scope (i.e. so you can use `<name>`
instead of `Module.<name>`), use this syntax:
```
scope (import "Module")
```
Note that the default prelude inclusion is equivalent to
```
scope (import "Prelude")
```

Here is an incomplete list of standard library modules:
 - Prelude (Either, IO, List, Optional, and String types; basic operators)
   - Automatically imported into every program, but can be excluded using the
     `no_prelude` directive (see above)
 - Algorithms (High-performance implementations of common algorithms)
 - DateTime (DateTime type and related functions)
   - Uses OS's time API to determine local current time
   - Allows actions to take place after a timeout
   - Allows accurate measurement of time intervals
 - Math (Fraction, Complex, and Integer types; more math functions)
 - Net (Network IO)
   - Provides APIs for UDP, TCP, HTTP, and HTTPS communication
   - Provides an easy-to-use HTTP server setup API
   - Provides an easy-to-use HTTP fetch API
 - Parse (JSON and XML parsers)
 - Random (Random number IO)
 - Regex (Regular expression type)
   - Uses regular expression syntax similar to JavaScript's
 - Structures (Implementations of less common data structures like graphs,
   filters, and trees)

##### Directives
Directives allow you to give instructions to the compiler or interpreter
directly. They are formed using the syntax `#[directive directive_name]` or
`#[directive directive_name directive_value]`. The former syntax is equivalent
to `#[directive directive_name true]`.

Here are some commonly used directives:
 - `deprecated` - Indicates a variable as deprecated.
 - `no_import` - If true, disables all imports (except the prelude).
 - `no_prelude` - If true, prevents Fleet from including the prelude.
 - `no_optimize` - If true, disables most optimizations of the interpreter or
    compiler.
 - `poison` - Invalidates a given identifier or syntactic construct.
 - `require_version` - Requires the given version string for Fleet's version.

##### Type Annotations
Although Fleet is statically typed, type annotations are usually optional. In
most cases, the compiler or interpreter can deduce the types you are using.
However, in some cases, there are multiple versions of a function that return
different types, in which case you must use type annotations. Type annotations
can also make your code more readable.

Type annotations can be written like so:
```
variable :: Type = value
```

They can also be written in function declarations:
```
function = parameter :: Type -> expression
```

You can also write annotations for types that are composed of other types:
```
variable :: MetaType Type = value
```

For functions, this looks like
```
function :: Type => Type = parameter :: Type -> expression
```

Generic type parameters are also available in functions:
```
function = parameter :: Any -> otherParameter :: concrete parameter ->
  expression
```

You can use multiple generic type parameters with one function:
```
function = parameter :: Any -> otherParameter :: Any -> expression
```

Types are first-class objects, so you can create types composed of other types:
```
TypeAOrTypeB = TypeA | TypeB
TypeAAndTypeB = TypeA & TypeB
```

You can also create types based on constraints:
```
HasFooFunction = providing (T :: Type -> Foo :: T => T)
```

The `new_type` expression provides a way to capture its lexical context,
including parameters passed to enclosing functions:
```
Constructor = x :: Integer -> y :: Integer -> new_type
one = Constructor 1 1
two = Constructor 2 2

one :: Constructor 1 1 # OK
two :: Constructor 1 2 # Produces an error

Constructor 1 some_y = one # OK: some_y is now 1
Constructor 1 some_y = two # Produces an error
```
