#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include "Evaluator.hpp"
#include "TokenStream.hpp"
#include "TokenTree.hpp"

std::vector<std::string> parseArguments(int argc, char **argv) {
  std::vector<std::string> arguments;
  for (int i = 0; i < argc; i++) {
    arguments.push_back(std::string(argv[i]));
  }
  return arguments;
}

int main(int argc, char **argv) {
  std::vector<std::string> arguments = parseArguments(argc, argv);
  if (arguments.size() == 2 && arguments.at(1) == "--version") {
    std::cout << "Fleet v0.0.1\nCreated by Thomas Smith\n";
    return 0;
  }
  else if (arguments.size() == 3 && arguments.at(1) == "-c") {
    TokenStream tokens { arguments.at(2) };
    TokenTree tree = TokenTree::build(tokens);
    Evaluator eval { Context::Pointer { new Context() } };
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
  else {
    std::string executableName {
      arguments.size() >= 1 ? arguments.at(0) : "<executable>"
    };
    std::cout << "Usage: " << executableName << " [--version] [-c code]\n";
    return 1;
  }
}
