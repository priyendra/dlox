#pragma once

#include <fmt/format.h>
#include <string>
#include "token-type.hpp"

namespace lox {

// Class that representes a Lox language token. These are produced by the
// scanner class. Tokens consist of type, lexeme and location.
class Token {
public:
  Token(TokenType type, std::string lexeme, int location)
      : type_(type), lexeme_(lexeme), location_(location) {}
  std::string debugString() {
    return fmt::format("{{type: {}, lexeme: '{}', location: {}}}",
                       tokenTypeToString(type_), lexeme_, location_);
  }

  TokenType type() const { return type_; }
  int location() const { return location_; }
  const std::string& lexeme() const { return lexeme_; }

private:
  TokenType type_;
  std::string lexeme_;
  int location_;
};

}  // namespace lox
