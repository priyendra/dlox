#include <fmt/format.h>
#include <memory>
#include <string>
#include <utility>

#include "ast.hpp"
#include "error-reporter.hpp"
#include "scanner.hpp"
#include "token.hpp"
#include "token-type.hpp"

namespace lox {

// This class implements a recursive descent parser that will parse the
// following grammar.
//
// expression     -> equality
// equality       -> comparison ( ( "!=" | "==" ) comparison )*
// comparison     -> addition ( ( ">" | ">=" | "<" | "<=" ) addition )*
// addition       -> multiplication ( ( "-" | "+" ) multiplication )*
// multiplication -> unary ( ( "/" | "*" ) unary )*
// unary          -> (( "!" | "-" ) unary) | primary
// primary        -> NUMBER | STRING | FALSE | TRUE | NIL | "(" expression ")"
template <typename Iterator> class Parser {
 public:
  Parser(ErrorReporter* err, Iterator begin, Iterator end)
    : err_(err), s_(err, begin, end) {}

  std::unique_ptr<ast::Node> parse() {
    try {
      auto node = parseExpression();
      if (not s_.done()) {
        err_->report(s_.currLocation(), "Unexpected unparsed input at end");
        return nullptr;
      }
      return node;
    } catch (ErrorReporter::Error err) {
      err_->report(err.location, err.msg);
    }
    return nullptr;
  }

 private:
  std::unique_ptr<ast::Node> parseExpression() { return parseEquality(); }
  std::unique_ptr<ast::Node> parseEquality() {
    auto node = parseComparison();
    while (true) {
      auto token = s_.next();
      if (token.type() == TokenType::END_OF_FILE) break;
      auto binary = std::make_unique<ast::Binary>();
      switch (token.type()) {
        case TokenType::BANG_EQUAL:
          binary->op = ast::Binary::BANG_EQUAL;
          break;
        case TokenType::EQUAL_EQUAL:
          binary->op = ast::Binary::EQUAL_EQUAL;
          break;
        default:
          s_.rewind();
          return node;
      }
      binary->first = std::move(node);
      binary->second = parseComparison();
      node = std::move(binary);
    }
    return node;
  }
  std::unique_ptr<ast::Node> parseComparison() {
    auto node = parseAddition();
    while (true) {
      auto token = s_.next();
      if (token.type() == TokenType::END_OF_FILE) break;
      auto binary = std::make_unique<ast::Binary>();
      switch (token.type()) {
        case TokenType::GREATER:
          binary->op = ast::Binary::GREATER;
          break;
        case TokenType::GREATER_EQUAL:
          binary->op = ast::Binary::GREATER_EQUAL;
          break;
        case TokenType::LESS:
          binary->op = ast::Binary::LESS;
          break;
        case TokenType::LESS_EQUAL:
          binary->op = ast::Binary::LESS_EQUAL;
          break;
        default:
          s_.rewind();
          return node;
      }
      binary->first = std::move(node);
      binary->second = parseAddition();
      node = std::move(binary);
    }
    return node;
  }
  std::unique_ptr<ast::Node> parseAddition() {
    auto node = parseMultiplication();
    while (true) {
      auto token = s_.next();
      if (token.type() == TokenType::END_OF_FILE) break;
      auto binary = std::make_unique<ast::Binary>();
      switch (token.type()) {
        case TokenType::MINUS:
          binary->op = ast::Binary::MINUS;
          break;
        case TokenType::PLUS:
          binary->op = ast::Binary::PLUS;
          break;
        default:
          s_.rewind();
          return node;
      }
      binary->first = std::move(node);
      binary->second = parseMultiplication();
      node = std::move(binary);
    }
    return node;
  }
  std::unique_ptr<ast::Node> parseMultiplication() {
    auto node = parseUnary();
    while (true) {
      auto token = s_.next();
      if (token.type() == TokenType::END_OF_FILE) break;
      auto binary = std::make_unique<ast::Binary>();
      switch (token.type()) {
        case TokenType::SLASH:
          binary->op = ast::Binary::SLASH;
          break;
        case TokenType::STAR:
          binary->op = ast::Binary::STAR;
          break;
        default:
          s_.rewind();
          return node;
      }
      binary->first = std::move(node);
      binary->second = parseUnary();
      node = std::move(binary);
    }
    return node;
  }
  std::unique_ptr<ast::Node> parseUnary() {
    auto token = s_.next();
    if (token.type() == TokenType::BANG || token.type() == TokenType::MINUS) {
      auto unary = std::make_unique<ast::Unary>();
      unary->op = token.type() == TokenType::BANG ? ast::Unary::BANG :
                                                    ast::Unary::MINUS;
      unary->operand = parseUnary();
      return unary;
    }
    s_.rewind();  // return the last token back to the scanner.
    return parsePrimary();
  }
  std::unique_ptr<ast::Node> parsePrimary() {
    auto token = s_.next();
    if (token.type() == TokenType::NUMBER) {
      auto number = std::make_unique<ast::Number>();
      try {
        number->val = std::stod(token.lexeme());
      } catch (...) {
        throw ErrorReporter::Error{
            token.location(),
            fmt::format("Invalid numeric literal: {}", token.lexeme())};
      }
      return number;
    }
    if (token.type() == TokenType::STRING) {
      auto str = std::make_unique<ast::String>();
      str->val = token.lexeme();
      return str;
    }
    if (token.type() == TokenType::FALSE) {
      auto boolean = std::make_unique<ast::Bool>();
      boolean->val = false;
      return boolean;
    }
    if (token.type() == TokenType::TRUE) {
      auto boolean = std::make_unique<ast::Bool>();
      boolean->val = true;
      return boolean;
    }
    if (token.type() == TokenType::NIL) return std::make_unique<ast::Nil>();
    if (token.type() == TokenType::LEFT_PAREN) {
      auto expr = parseExpression();
      token = s_.next();  // consume closing ')'
      if (token.type() != TokenType::RIGHT_PAREN) {
        throw ErrorReporter::Error{
            token.location(),
            fmt::format("Expecting right paren, found: {}", token.lexeme())};
      }
      return expr;
    }
    throw ErrorReporter::Error{
        token.location(),
        fmt::format("Unexpected token: {}", token.debugString())};
  }
  ErrorReporter* err_;
  Scanner<Iterator> s_;
};

}  // namespace lox
