#include <gtest/gtest.h>
#include <string>

#include "ast-printer.hpp"
#include "parser.hpp"

namespace lox {

TEST(Parser, Number) {
  {
    std::string expr = "123";
    ErrorReporter err(expr);
    Parser parser(&err, expr.begin(), expr.end());
    auto parsed = parser.parse();
    EXPECT_FALSE(err.hasErrors());
    EXPECT_EQ("123", ast::Printer::print(parsed.get()));
  }
  {
    std::string expr = "123.5";
    ErrorReporter err(expr);
    Parser parser(&err, expr.begin(), expr.end());
    auto parsed = parser.parse();
    EXPECT_FALSE(err.hasErrors());
    EXPECT_EQ("123.5", ast::Printer::print(parsed.get()));
  }
  {
    std::string expr = "123 + 456.5";
    ErrorReporter err(expr);
    Parser parser(&err, expr.begin(), expr.end());
    auto parsed = parser.parse();
    EXPECT_FALSE(err.hasErrors());
    EXPECT_EQ("(+ 123, 456.5)", ast::Printer::print(parsed.get()));
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
  }
  {
    std::string expr = "\"12\\\"3\"";
    ErrorReporter err(expr);
    Parser parser(&err, expr.begin(), expr.end());
    auto parsed = parser.parse();
    EXPECT_FALSE(err.hasErrors());
    EXPECT_EQ("'12\"3'", ast::Printer::print(parsed.get()));
  }
  {
    std::string expr = "\"123\" > \"456\"";
    ErrorReporter err(expr);
    Parser parser(&err, expr.begin(), expr.end());
    auto parsed = parser.parse();
    EXPECT_FALSE(err.hasErrors());
    EXPECT_EQ("(> '123', '456')", ast::Printer::print(parsed.get()));
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
  }
  {
    std::string expr = "false";
    ErrorReporter err(expr);
    Parser parser(&err, expr.begin(), expr.end());
    auto parsed = parser.parse();
    EXPECT_FALSE(err.hasErrors());
    EXPECT_EQ("false", ast::Printer::print(parsed.get()));
  }
  {
    std::string expr = "true == false";
    ErrorReporter err(expr);
    Parser parser(&err, expr.begin(), expr.end());
    auto parsed = parser.parse();
    EXPECT_FALSE(err.hasErrors());
    EXPECT_EQ("(== true, false)", ast::Printer::print(parsed.get()));
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
  }
  {
    std::string expr = "nil != nil";
    ErrorReporter err(expr);
    Parser parser(&err, expr.begin(), expr.end());
    auto parsed = parser.parse();
    EXPECT_FALSE(err.hasErrors());
    EXPECT_EQ("(!= nil, nil)", ast::Printer::print(parsed.get()));
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
}

TEST(Parser, ComplexTwo) {
  std::string expr = "-------2";
  ErrorReporter err(expr);
  Parser parser(&err, expr.begin(), expr.end());
  auto parsed = parser.parse();
  EXPECT_FALSE(err.hasErrors());
  EXPECT_EQ(
      "(- (- (- (- (- (- (- 2)))))))", ast::Printer::print(parsed.get()));
}

}  // namespace lox

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
