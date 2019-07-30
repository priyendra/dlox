#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace lox {

// Simple utility class to collect errors as recorded by the Lox interpreter.
class ErrorReporter {
 public:
  struct Error {
    int location;
    std::string msg;
  };
  ErrorReporter(const std::string& src) : src_(src) {}
  void report(int location, const std::string& msg) {
    errors_.push_back({location, msg});
  }
  bool hasErrors() const { return errors_.size() > 0; }
  int numErrors() const { return errors_.size(); }
  const Error& error(int i) const { return errors_[i]; }

 private:
  std::string src_;
  std::vector<Error> errors_;
};

}  // namespace lox
