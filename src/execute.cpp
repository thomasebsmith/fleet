// File: src/execute.cpp
// Purpose: This file contains the entry point for the main Fleet executable
//  (i.e. it contains the main function). It parses command line arguments
//  and appropriately executes Fleet code.

#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include "DefaultContext.hpp"
#include "Evaluator.hpp"
#include "TokenStream.hpp"
#include "TokenTree.hpp"

// parseArguments(argc, argv) - Creates a vector of strings from the raw
//  command line arguments.
std::vector<std::string> parseArguments(int argc, char **argv) {
  std::vector<std::string> arguments;
  for (int i = 0; i < argc; i++) {
    arguments.push_back(std::string(argv[i]));
  }
  return arguments;
}

// main(argc, argv) - The entry point for the main Fleet executable.
// Command line syntax:
//  executable_name [--version | -c code | -t code ]
//  --version - Prints the version of Fleet being used and the author's name.
//  -c code   - Executes `code` and prints the result or an error.
//  -t code   - Creates an AST of `code` and prints its string representation.
//  (With any other syntax, usage help is printed).
int main(int argc, char **argv) {
  std::vector<std::string> arguments = parseArguments(argc, argv);
  if (arguments.size() == 2 && arguments.at(1) == "--version") {
    std::cout << "Fleet v0.0.1\nCreated by Thomas Smith\n";
    return 0;
  }
  else if (arguments.size() == 3 && arguments.at(1) == "-c") {
    TokenStream tokens { arguments.at(2) };
    TokenTree tree = TokenTree::build(tokens);
    Evaluator eval { Context::Pointer { new DefaultContext() } };
    Value::OrError result = eval.evaluate(tree);
    if (std::holds_alternative<Value::Pointer>(result)) {
      std::cout << static_cast<std::string>(
        **std::get_if<Value::Pointer>(&result)
      ) << "\n";
      return 0;
    }
    else {
      std::cout << "Error: " << std::get_if<std::runtime_error>(
        &result
      )->what() << "\n";
      return 1;
    }
  }
  else if (arguments.size() == 3 && arguments.at(1) == "-t") {
    TokenStream tokens { arguments.at(2) };
    TokenTree tree = TokenTree::build(tokens);
    std::cout << static_cast<std::string>(tree) << "\n";
  }
  else {
    std::string executableName {
      arguments.size() >= 1 ? arguments.at(0) : "<executable>"
    };
    std::cout << "Usage: " << executableName;
    std::cout << " [--version] [-c code] [-t code]\n";
    return 1;
  }
}
