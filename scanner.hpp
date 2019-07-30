#pragma once

#include <fmt/format.h>
#include <optional>
#include "error-reporter.hpp"
#include "token.hpp"

namespace lox {

// The Scanner class allows callers to lexically scan a piece of Lox code
// and convert it into a stream of tokens. Intended usage as follows:
//   std::vector<Token> tokens;
//   auto scanner = makeScanner(...);
//   while (true) {
//     auto token = scanner.next();
//     if (token.type() == TokenType::END_OF_FILE) break;
//     tokens.push_back(token);
//   }
template <typename Iterator> class Scanner {
 public:
  Scanner(ErrorReporter* err, Iterator begin, Iterator end)
    : err_(err), b_(begin), e_(end), i_(begin) {}
  // Returns the next token from the program. The last token will always be
  // of type TokenType::END_OF_FILE. It is illegal to call next() after it
  // has once returned EOF.
  Token next();

 private:
  Token scanIdentifier();
  Token scanNumber();
  Token scanString();

  ErrorReporter* err_;
  Iterator b_;
  Iterator e_;
  Iterator i_;
};

template <typename Iterator> Token Scanner<Iterator>::next() {
  while (i_ != e_) {
    auto c = *i_;
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_') {
      return scanIdentifier();
    } else if (c >= '0' && c <= '9') {
      return scanNumber();
    } else if (c == '"') {
      return scanString();
    } else if (c == ' ' || c == '\t' || c == '\n') {
      ++i_;  // skip whitespace
    } else if (c == '(') {
      return Token(TokenType::LEFT_PAREN, "(", i_++ - b_);
    } else if (c == ')') {
      return Token(TokenType::RIGHT_PAREN, ")", i_++ - b_);
    } else if (c == '{') {
      return Token(TokenType::LEFT_BRACE, "{", i_++ - b_);
    } else if (c == '}') {
      return Token(TokenType::RIGHT_BRACE, "}", i_++ - b_);
    } else if (c == ',') {
      return Token(TokenType::COMMA, ",", i_++ - b_);
    } else if (c == '.') {
      return Token(TokenType::DOT, ".", i_++ - b_);
    } else if (c == '-') {
      return Token(TokenType::MINUS, "-", i_++ - b_);
    } else if (c == '+') {
      return Token(TokenType::PLUS, "+", i_++ - b_);
    } else if (c == ';') {
      return Token(TokenType::SEMICOLON, ";", i_++ - b_);
    } else if (c == '/') {
      i_++;  // consume the /
      if (i_ == e_ || *i_ != '/') {  // not a comment.
        return Token(TokenType::SLASH, "/", i_ - 1 - b_);
      }
      while (i_ < e_ && *i_ != '\n') ++i_;  // skip the comment.
    } else if (c == '*') {
      return Token(TokenType::STAR, "*", i_++ - b_);
    } else if (c == '!') {
      i_++;
      if (i_ == e_ || *i_ != '=') {
        return Token(TokenType::BANG, "!", i_ - 1 - b_);
      }
      return Token(TokenType::BANG_EQUAL, "!=", i_++ - 1 - b_);
    } else if (c == '=') {
      i_++;
      if (i_ == e_ || *i_ != '=') {
        return Token(TokenType::EQUAL, "=", i_ - 1 - b_);
      }
      return Token(TokenType::EQUAL_EQUAL, "==", i_++ - 1 - b_);
    } else if (c == '>') {
      i_++;
      if (i_ == e_ || *i_ != '=') {
        return Token(TokenType::GREATER, ">", i_ - 1 - b_);
      }
      return Token(TokenType::GREATER_EQUAL, ">=", i_++ - 1 - b_);
    } else if (c == '<') {
      i_++;
      if (i_ == e_ || *i_ != '=') {
        return Token(TokenType::LESS, "<", i_ - 1 - b_);
      }
      return Token(TokenType::LESS_EQUAL, "<=", i_++ - 1 - b_);
    } else {
      err_->report(
          i_ - b_, fmt::format("Unexpected char: '{:c}'[0x{:x}]", *i_, *i_));
      i_++;  // skip unexpected character and continue scanning.
    }
  }
  return Token(TokenType::END_OF_FILE, "", e_ - b_);
}

template <typename Iterator> Token Scanner<Iterator>::scanIdentifier() {
  const int location = i_ - b_;
  std::string identifier;
  while (i_ < e_) {
    auto c = *i_;
    if ((c < 'A' || c > 'Z') && (c < 'a' || c > 'z') && c != '_' &&
        (c < '0' || c > '9')) {
      break;
    }
    identifier.push_back(c);
    ++i_;
  }
  if (identifier == "and") {
    return Token(TokenType::AND, identifier, location);
  } else if (identifier == "class") {
    return Token(TokenType::CLASS, identifier, location);
  } else if (identifier == "else") {
    return Token(TokenType::ELSE, identifier, location);
  } else if (identifier == "exit") {
    return Token(TokenType::EXIT, identifier, location);
  } else if (identifier == "false") {
    return Token(TokenType::FALSE, identifier, location);
  } else if (identifier == "fun") {
    return Token(TokenType::FUN, identifier, location);
  } else if (identifier == "for") {
    return Token(TokenType::FOR, identifier, location);
  } else if (identifier == "if") {
    return Token(TokenType::IF, identifier, location);
  } else if (identifier == "nil") {
    return Token(TokenType::NIL, identifier, location);
  } else if (identifier == "or") {
    return Token(TokenType::OR, identifier, location);
  } else if (identifier == "print") {
    return Token(TokenType::PRINT, identifier, location);
  } else if (identifier == "return") {
    return Token(TokenType::RETURN, identifier, location);
  } else if (identifier == "super") {
    return Token(TokenType::SUPER, identifier, location);
  } else if (identifier == "this") {
    return Token(TokenType::THIS, identifier, location);
  } else if (identifier == "true") {
    return Token(TokenType::TRUE, identifier, location);
  } else if (identifier == "var") {
    return Token(TokenType::VAR, identifier, location);
  } else if (identifier == "while") {
    return Token(TokenType::WHILE, identifier, location);
  }
  return Token(TokenType::IDENTIFIER, identifier, location);
}

template <typename Iterator> Token Scanner<Iterator>::scanNumber() {
  const int location = i_ - b_;
  std::string number;
  while (i_ < e_) {
    if (*i_ < '0' || *i_ > '9') break;
    number.push_back(*i_);
    ++i_;
  }
  if (i_ < e_ && *i_ == '.') {  // scan fractional part as well.
    number.push_back('.');
    ++i_;
    while (i_ < e_) {
      if (*i_ < '0' && *i_ > '9') break;
      number.push_back(*i_);
      ++i_;
    }
  }
  return Token(TokenType::NUMBER, number, location);
}

template <typename Iterator> Token Scanner<Iterator>::scanString() {
  const int location = i_ - b_;
  std::string str;
  i_++;  // consume beginning quote
  bool escaped = false;
  while (i_ < e_) {
    auto c = *i_++;
    const bool escapedQuote = not escaped && c == '"';
    escaped = false;
    if (escapedQuote) {
      return Token(TokenType::STRING, str, location);
    } else if (c == '\\') {
      escaped = true;
    } else {
      str.push_back(c);
    }
  }
  err_->report(location, "Unterminated string");
  return Token(TokenType::STRING, str, location);
}

}  // namespace lox
