#include <fstream>
#include <iostream>
#include <string>
#include <gflags/gflags.h>

#include "ast-printer.hpp"
#include "error-reporter.hpp"
#include "parser.hpp"
#include "token.hpp"

namespace lox {

namespace {

bool readFile(const char* filename, std::string* out) {
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (not in) return false;
  try {
    in.seekg(0, std::ios::end);
    out->resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&(*out)[0], out->size());
  } catch (...) {
    return false;
  }
  return true;
}

}  // namespace

class LoxEngine {
 public:
  bool runInteractive() {
    while (std::cin.good()) {
      std::string program;
      std::cout << "> ";
      // TODO: Support multi-line inputs perhaps escaped via backslash.
      std::getline(std::cin, program);
      // TODO: Implement exit as a proper part of the language.
      if (program == "exit") return true;
      run(program);
    }
    return false;
  }
  bool runFile(const char* filename) {
    std::string program;
    if (not readFile(filename, &program)) return false;
    return run(program);
  }

 private:
  bool run(const std::string& src) {
    ErrorReporter err(src);
    Parser parser(&err, src.begin(), src.end());
    auto parsed = parser.parse();
    if (err.hasErrors()) {
      for (int i = 0; i < err.numErrors(); ++i) {
        std::cout << "Error: location=" << err.error(i).location
                  << ", msg=" << err.error(i).msg << std::endl;
      }
    } else {
      std::cout << "AST: " << ast::Printer::print(parsed.get(), true)
                << std::endl;
    }
    return true;
  }
};

void printUsage() {
  const char* usage =
      "lox: Interpreter to run programs written in the lox programming "
      "language.\n"
      "Usage: $ lox [PROGRAM]\n"
      "  PROGRAM: Optional argument for the path of the program to execute.\n"
      "           If missing, the interpreter will run in interactive "
      "mode.\n";
  std::cerr << usage;
}

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  LoxEngine engine;
  switch (argc) {
    case 1:
      return engine.runInteractive() ? 0 : 1;
    case 2:
      return engine.runFile(argv[1]) ? 0 : 65;
    default:
      printUsage();
      return 64;
  }
  return 1;
}

}  // namespace lox

int main(int argc, char** argv) { return lox::main(argc, argv); }
