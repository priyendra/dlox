#include <gtest/gtest.h>
#include <string>
#include <type_traits>

#include "ast-eval.hpp"
#include "ast-printer.hpp"
#include "parser.hpp"

namespace lox {

void MatchNil(ast::Node* node) {
  ast::Value value;
  auto status = ast::Evaluator::eval(node, &value);
  EXPECT_TRUE(status.ok);
  EXPECT_EQ(ast::ValueType::NIL, value.type());
}

void MatchBool(ast::Node* node, bool b) {
  ast::Value value;
  auto status = ast::Evaluator::eval(node, &value);
  EXPECT_TRUE(status.ok);
  EXPECT_EQ(ast::ValueType::BOOL, value.type());
  EXPECT_EQ(b, value.b());
}

void MatchDouble(ast::Node* node, double d) {
  ast::Value value;
  auto status = ast::Evaluator::eval(node, &value);
  EXPECT_TRUE(status.ok);
  EXPECT_EQ(ast::ValueType::NUMBER, value.type());
  EXPECT_EQ(d, value.d());
}

void MatchString(ast::Node* node, std::string s) {
  ast::Value value;
  auto status = ast::Evaluator::eval(node, &value);
  EXPECT_TRUE(status.ok);
  EXPECT_EQ(ast::ValueType::STRING, value.type());
  EXPECT_EQ(s, value.s());
}

void MatchError(ast::Node* node) {
  ast::Value value;
  auto status = ast::Evaluator::eval(node, &value);
  EXPECT_FALSE(status.ok);
}

TEST(Parser, Number) {
  {
    std::string expr = "123";
    ErrorReporter err(expr);
    Parser parser(&err, expr.begin(), expr.end());
    auto parsed = parser.parse();
    EXPECT_FALSE(err.hasErrors());
    EXPECT_EQ("123", ast::Printer::print(parsed.get()));
    MatchDouble(parsed.get(), 123.0);
  }
  {
    std::string expr = "123.5";
    ErrorReporter err(expr);
    Parser parser(&err, expr.begin(), expr.end());
    auto parsed = parser.parse();
    EXPECT_FALSE(err.hasErrors());
    EXPECT_EQ("123.5", ast::Printer::print(parsed.get()));
    MatchDouble(parsed.get(), 123.5);
  }
  {
    std::string expr = "123 + 456.5";
    ErrorReporter err(expr);
    Parser parser(&err, expr.begin(), expr.end());
    auto parsed = parser.parse();
    EXPECT_FALSE(err.hasErrors());
    EXPECT_EQ("(+ 123, 456.5)", ast::Printer::print(parsed.get()));
    MatchDouble(parsed.get(), 579.5);
  }
}

TEST(Parser, String) {
  {
    std::string expr = "\"123\"";
    ErrorReporter err(expr);
    Parser parser(&err, expr.begin(), expr.end());
    auto parsed = parser.parse();
    EXPECT_FALSE(err.hasErrors());
    EXPECT_EQ("'123'", ast::Printer::print(parsed.get()));
    MatchString(parsed.get(), "123");
  }
  {
    std::string expr = "\"12\\\"3\"";
    ErrorReporter err(expr);
    Parser parser(&err, expr.begin(), expr.end());
    auto parsed = parser.parse();
    EXPECT_FALSE(err.hasErrors());
    EXPECT_EQ("'12\"3'", ast::Printer::print(parsed.get()));
    MatchString(parsed.get(), "12\"3");
  }
  {
    std::string expr = "\"123\" > \"456\"";
    ErrorReporter err(expr);
    Parser parser(&err, expr.begin(), expr.end());
    auto parsed = parser.parse();
    EXPECT_FALSE(err.hasErrors());
    EXPECT_EQ("(> '123', '456')", ast::Printer::print(parsed.get()));
    MatchError(parsed.get());
  }
}

TEST(Parser, Bool) {
  {
    std::string expr = "true";
    ErrorReporter err(expr);
    Parser parser(&err, expr.begin(), expr.end());
    auto parsed = parser.parse();
    EXPECT_FALSE(err.hasErrors());
    EXPECT_EQ("true", ast::Printer::print(parsed.get()));
    MatchBool(parsed.get(), true);
  }
  {
    std::string expr = "false";
    ErrorReporter err(expr);
    Parser parser(&err, expr.begin(), expr.end());
    auto parsed = parser.parse();
    EXPECT_FALSE(err.hasErrors());
    EXPECT_EQ("false", ast::Printer::print(parsed.get()));
    MatchBool(parsed.get(), false);
  }
  {
    std::string expr = "true == false";
    ErrorReporter err(expr);
    Parser parser(&err, expr.begin(), expr.end());
    auto parsed = parser.parse();
    EXPECT_FALSE(err.hasErrors());
    EXPECT_EQ("(== true, false)", ast::Printer::print(parsed.get()));
    MatchBool(parsed.get(), false);
  }
}

TEST(Parser, Nil) {
  {
    std::string expr = "nil";
    ErrorReporter err(expr);
    Parser parser(&err, expr.begin(), expr.end());
    auto parsed = parser.parse();
    EXPECT_FALSE(err.hasErrors());
    EXPECT_EQ("nil", ast::Printer::print(parsed.get()));
    MatchNil(parsed.get());
  }
  {
    std::string expr = "nil != nil";
    ErrorReporter err(expr);
    Parser parser(&err, expr.begin(), expr.end());
    auto parsed = parser.parse();
    EXPECT_FALSE(err.hasErrors());
    EXPECT_EQ("(!= nil, nil)", ast::Printer::print(parsed.get()));
    MatchBool(parsed.get(), false);
  }
}

TEST(Parser, ComplexOne) {
  std::string expr = "1/2 + 1 > (3 + 5) == -4 <= 100 - 4*2";
  ErrorReporter err(expr);
  Parser parser(&err, expr.begin(), expr.end());
  auto parsed = parser.parse();
  EXPECT_FALSE(err.hasErrors());
  EXPECT_EQ(
      "(== (> (+ (/ 1, 2), 1), (+ 3, 5)), (<= (- 4), (- 100, (* 4, 2))))",
      ast::Printer::print(parsed.get()));
  MatchBool(parsed.get(), false);
}

TEST(Parser, ComplexTwo) {
  std::string expr = "-------2";
  ErrorReporter err(expr);
  Parser parser(&err, expr.begin(), expr.end());
  auto parsed = parser.parse();
  EXPECT_FALSE(err.hasErrors());
  EXPECT_EQ(
      "(- (- (- (- (- (- (- 2)))))))", ast::Printer::print(parsed.get()));
  MatchDouble(parsed.get(), -2.0);
}

}  // namespace lox

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
