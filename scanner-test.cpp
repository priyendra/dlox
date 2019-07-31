#include "error-reporter.hpp"
#include "scanner.hpp"
#include "token.hpp"
#include <gtest/gtest.h>

namespace lox {

// TODO: Add more test cases for negative cases (malformed input).

class ScanHarness {
 public:
  ScanHarness(const std::string& src)
    : src_(src), err_(src), s_(&err_, src_.begin(), src_.end()) {}
  std::vector<Token> scanAll() {
    std::vector<Token> tokens;
    while (true) {
      auto token = s_.next();
      tokens.push_back(token);
      if (token.type() == TokenType::END_OF_FILE) break;
    }
    return tokens;
  }
  bool hasErrors() const { return err_.hasErrors(); }
  bool numErrors() const { return err_.numErrors(); }
  const ErrorReporter::Error& error(int i) const { return err_.error(i); }

 private:
  std::string src_;
  ErrorReporter err_;
  Scanner<std::string::const_iterator> s_;
};

TEST(Scanner, Identifiers) {
  {
    ScanHarness s("hello");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::IDENTIFIER, tokens[0].type());
    EXPECT_EQ("hello", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("hello1");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::IDENTIFIER, tokens[0].type());
    EXPECT_EQ("hello1", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("h3llo1");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::IDENTIFIER, tokens[0].type());
    EXPECT_EQ("h3llo1", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("hello123");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::IDENTIFIER, tokens[0].type());
    EXPECT_EQ("hello123", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("hello_world_bye");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::IDENTIFIER, tokens[0].type());
    EXPECT_EQ("hello_world_bye", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("hello_world_12bye");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::IDENTIFIER, tokens[0].type());
    EXPECT_EQ("hello_world_12bye", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("hello123 world5");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(3, tokens.size());
    EXPECT_EQ(TokenType::IDENTIFIER, tokens[0].type());
    EXPECT_EQ("hello123", tokens[0].lexeme());
    EXPECT_EQ(TokenType::IDENTIFIER, tokens[1].type());
    EXPECT_EQ("world5", tokens[1].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[2].type());
  }
}

TEST(Scanner, Keywords) {
  {
    ScanHarness s("and");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::AND, tokens[0].type());
    EXPECT_EQ("and", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("class");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::CLASS, tokens[0].type());
    EXPECT_EQ("class", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("else");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::ELSE, tokens[0].type());
    EXPECT_EQ("else", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("exit");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::EXIT, tokens[0].type());
    EXPECT_EQ("exit", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("false");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::FALSE, tokens[0].type());
    EXPECT_EQ("false", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("fun");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::FUN, tokens[0].type());
    EXPECT_EQ("fun", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("for");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::FOR, tokens[0].type());
    EXPECT_EQ("for", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("if");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::IF, tokens[0].type());
    EXPECT_EQ("if", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("nil");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::NIL, tokens[0].type());
    EXPECT_EQ("nil", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("or");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::OR, tokens[0].type());
    EXPECT_EQ("or", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("print");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::PRINT, tokens[0].type());
    EXPECT_EQ("print", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("return");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::RETURN, tokens[0].type());
    EXPECT_EQ("return", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("super");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::SUPER, tokens[0].type());
    EXPECT_EQ("super", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("this");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::THIS, tokens[0].type());
    EXPECT_EQ("this", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("true");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::TRUE, tokens[0].type());
    EXPECT_EQ("true", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("var");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::VAR, tokens[0].type());
    EXPECT_EQ("var", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("while");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::WHILE, tokens[0].type());
    EXPECT_EQ("while", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
}

TEST(Scanner, Numbers) {
  {
    ScanHarness s("123");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::NUMBER, tokens[0].type());
    EXPECT_EQ("123", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("123.");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::NUMBER, tokens[0].type());
    EXPECT_EQ("123.", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("123.456");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::NUMBER, tokens[0].type());
    EXPECT_EQ("123.456", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("-123.456");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(3, tokens.size());
    EXPECT_EQ(TokenType::MINUS, tokens[0].type());
    EXPECT_EQ("-", tokens[0].lexeme());
    EXPECT_EQ(TokenType::NUMBER, tokens[1].type());
    EXPECT_EQ("123.456", tokens[1].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[2].type());
  }
}

TEST(Scanner, Strings) {
  {
    ScanHarness s("\"hello world\"");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::STRING, tokens[0].type());
    EXPECT_EQ("hello world", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
  {
    ScanHarness s("\"hello \\\" world\"");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ(TokenType::STRING, tokens[0].type());
    EXPECT_EQ("hello \" world", tokens[0].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[1].type());
  }
}

TEST(Scanner, WhiteSpace) {
  {
    ScanHarness s("hello\tworld");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(3, tokens.size());
    EXPECT_EQ(TokenType::IDENTIFIER, tokens[0].type());
    EXPECT_EQ("hello", tokens[0].lexeme());
    EXPECT_EQ(TokenType::IDENTIFIER, tokens[1].type());
    EXPECT_EQ("world", tokens[1].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[2].type());
  }
  {
    ScanHarness s("hello\tworld");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(3, tokens.size());
    EXPECT_EQ(TokenType::IDENTIFIER, tokens[0].type());
    EXPECT_EQ("hello", tokens[0].lexeme());
    EXPECT_EQ(TokenType::IDENTIFIER, tokens[1].type());
    EXPECT_EQ("world", tokens[1].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[2].type());
  }
  {
    ScanHarness s("hello\nworld");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(3, tokens.size());
    EXPECT_EQ(TokenType::IDENTIFIER, tokens[0].type());
    EXPECT_EQ("hello", tokens[0].lexeme());
    EXPECT_EQ(TokenType::IDENTIFIER, tokens[1].type());
    EXPECT_EQ("world", tokens[1].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[2].type());
  }
  {
    ScanHarness s("  \thello\t\n\nworld   \n  ");
    auto tokens = s.scanAll();
    EXPECT_FALSE(s.hasErrors());
    EXPECT_EQ(3, tokens.size());
    EXPECT_EQ(TokenType::IDENTIFIER, tokens[0].type());
    EXPECT_EQ("hello", tokens[0].lexeme());
    EXPECT_EQ(TokenType::IDENTIFIER, tokens[1].type());
    EXPECT_EQ("world", tokens[1].lexeme());
    EXPECT_EQ(TokenType::END_OF_FILE, tokens[2].type());
  }
}

TEST(Scanner, Operators) {
  ScanHarness s("( ) { } , . - + ; / * ! != = == > >= < <=");
  auto tokens = s.scanAll();
  EXPECT_FALSE(s.hasErrors());
  EXPECT_EQ(20, tokens.size());
  EXPECT_EQ(TokenType::LEFT_PAREN, tokens[0].type());
  EXPECT_EQ("(", tokens[0].lexeme());
  EXPECT_EQ(TokenType::RIGHT_PAREN, tokens[1].type());
  EXPECT_EQ(")", tokens[1].lexeme());
  EXPECT_EQ(TokenType::LEFT_BRACE, tokens[2].type());
  EXPECT_EQ("{", tokens[2].lexeme());
  EXPECT_EQ(TokenType::RIGHT_BRACE, tokens[3].type());
  EXPECT_EQ("}", tokens[3].lexeme());
  EXPECT_EQ(TokenType::COMMA, tokens[4].type());
  EXPECT_EQ(",", tokens[4].lexeme());
  EXPECT_EQ(TokenType::DOT, tokens[5].type());
  EXPECT_EQ(".", tokens[5].lexeme());
  EXPECT_EQ(TokenType::MINUS, tokens[6].type());
  EXPECT_EQ("-", tokens[6].lexeme());
  EXPECT_EQ(TokenType::PLUS, tokens[7].type());
  EXPECT_EQ("+", tokens[7].lexeme());
  EXPECT_EQ(TokenType::SEMICOLON, tokens[8].type());
  EXPECT_EQ(";", tokens[8].lexeme());
  EXPECT_EQ(TokenType::SLASH, tokens[9].type());
  EXPECT_EQ("/", tokens[9].lexeme());
  EXPECT_EQ(TokenType::STAR, tokens[10].type());
  EXPECT_EQ("*", tokens[10].lexeme());
  EXPECT_EQ(TokenType::BANG, tokens[11].type());
  EXPECT_EQ("!", tokens[11].lexeme());
  EXPECT_EQ(TokenType::BANG_EQUAL, tokens[12].type());
  EXPECT_EQ("!=", tokens[12].lexeme());
  EXPECT_EQ(TokenType::EQUAL, tokens[13].type());
  EXPECT_EQ("=", tokens[13].lexeme());
  EXPECT_EQ(TokenType::EQUAL_EQUAL, tokens[14].type());
  EXPECT_EQ("==", tokens[14].lexeme());
  EXPECT_EQ(TokenType::GREATER, tokens[15].type());
  EXPECT_EQ(">", tokens[15].lexeme());
  EXPECT_EQ(TokenType::GREATER_EQUAL, tokens[16].type());
  EXPECT_EQ(">=", tokens[16].lexeme());
  EXPECT_EQ(TokenType::LESS, tokens[17].type());
  EXPECT_EQ("<", tokens[17].lexeme());
  EXPECT_EQ(TokenType::LESS_EQUAL, tokens[18].type());
  EXPECT_EQ("<=", tokens[18].lexeme());
  EXPECT_EQ(TokenType::END_OF_FILE, tokens[19].type());
}

TEST(Scanner, UnexpectedCharacters) {
  ScanHarness s("hello # world");
  auto tokens = s.scanAll();
  EXPECT_EQ(1, s.numErrors());
  EXPECT_EQ(6, s.error(0).location);
  EXPECT_EQ("Unexpected char: '#'[0x23]", s.error(0).msg);
  EXPECT_EQ(3, tokens.size());
  EXPECT_EQ(TokenType::IDENTIFIER, tokens[0].type());
  EXPECT_EQ("hello", tokens[0].lexeme());
  EXPECT_EQ(TokenType::IDENTIFIER, tokens[1].type());
  EXPECT_EQ("world", tokens[1].lexeme());
  EXPECT_EQ(TokenType::END_OF_FILE, tokens[2].type());
}

TEST(Scanner, WithComments) {
  ScanHarness s("// comment\nhello world\n// another comment");
  auto tokens = s.scanAll();
  EXPECT_FALSE(s.hasErrors());
  EXPECT_EQ(3, tokens.size());
  EXPECT_EQ(TokenType::IDENTIFIER, tokens[0].type());
  EXPECT_EQ("hello", tokens[0].lexeme());
  EXPECT_EQ(TokenType::IDENTIFIER, tokens[1].type());
  EXPECT_EQ("world", tokens[1].lexeme());
  EXPECT_EQ(TokenType::END_OF_FILE, tokens[2].type());
}

TEST(Scanner, MultiLineProgram) {
  const char* kSrc =
    "var v1 = -100;\n"
    "while (v1 < 200) {\n"
    "  print \"Hello\";\n"
    "  print v1;\n"
    "  v1 = v1 + 1;\n"
    "}\n";
  ScanHarness s(kSrc);
  auto tokens = s.scanAll();
  EXPECT_FALSE(s.hasErrors());
  EXPECT_EQ(27, tokens.size());
  EXPECT_EQ(TokenType::VAR, tokens[0].type());
  EXPECT_EQ("var", tokens[0].lexeme());
  EXPECT_EQ(TokenType::IDENTIFIER, tokens[1].type());
  EXPECT_EQ("v1", tokens[1].lexeme());
  EXPECT_EQ(TokenType::EQUAL, tokens[2].type());
  EXPECT_EQ("=", tokens[2].lexeme());
  EXPECT_EQ(TokenType::MINUS, tokens[3].type());
  EXPECT_EQ("-", tokens[3].lexeme());
  EXPECT_EQ(TokenType::NUMBER, tokens[4].type());
  EXPECT_EQ("100", tokens[4].lexeme());
  EXPECT_EQ(TokenType::SEMICOLON, tokens[5].type());
  EXPECT_EQ(";", tokens[5].lexeme());
  EXPECT_EQ(TokenType::WHILE, tokens[6].type());
  EXPECT_EQ("while", tokens[6].lexeme());
  EXPECT_EQ(TokenType::LEFT_PAREN, tokens[7].type());
  EXPECT_EQ("(", tokens[7].lexeme());
  EXPECT_EQ(TokenType::IDENTIFIER, tokens[8].type());
  EXPECT_EQ("v1", tokens[8].lexeme());
  EXPECT_EQ(TokenType::LESS, tokens[9].type());
  EXPECT_EQ("<", tokens[9].lexeme());
  EXPECT_EQ(TokenType::NUMBER, tokens[10].type());
  EXPECT_EQ("200", tokens[10].lexeme());
  EXPECT_EQ(TokenType::RIGHT_PAREN, tokens[11].type());
  EXPECT_EQ(")", tokens[11].lexeme());
  EXPECT_EQ(TokenType::LEFT_BRACE, tokens[12].type());
  EXPECT_EQ("{", tokens[12].lexeme());
  EXPECT_EQ(TokenType::PRINT, tokens[13].type());
  EXPECT_EQ("print", tokens[13].lexeme());
  EXPECT_EQ(TokenType::STRING, tokens[14].type());
  EXPECT_EQ("Hello", tokens[14].lexeme());
  EXPECT_EQ(TokenType::SEMICOLON, tokens[15].type());
  EXPECT_EQ(";", tokens[15].lexeme());
  EXPECT_EQ(TokenType::PRINT, tokens[16].type());
  EXPECT_EQ("print", tokens[16].lexeme());
  EXPECT_EQ(TokenType::IDENTIFIER, tokens[17].type());
  EXPECT_EQ("v1", tokens[17].lexeme());
  EXPECT_EQ(TokenType::SEMICOLON, tokens[18].type());
  EXPECT_EQ(";", tokens[18].lexeme());
  EXPECT_EQ(TokenType::IDENTIFIER, tokens[19].type());
  EXPECT_EQ("v1", tokens[19].lexeme());
  EXPECT_EQ(TokenType::EQUAL, tokens[20].type());
  EXPECT_EQ("=", tokens[20].lexeme());
  EXPECT_EQ(TokenType::IDENTIFIER, tokens[21].type());
  EXPECT_EQ("v1", tokens[21].lexeme());
  EXPECT_EQ(TokenType::PLUS, tokens[22].type());
  EXPECT_EQ("+", tokens[22].lexeme());
  EXPECT_EQ(TokenType::NUMBER, tokens[23].type());
  EXPECT_EQ("1", tokens[23].lexeme());
  EXPECT_EQ(TokenType::SEMICOLON, tokens[24].type());
  EXPECT_EQ(";", tokens[24].lexeme());
  EXPECT_EQ(TokenType::RIGHT_BRACE, tokens[25].type());
  EXPECT_EQ("}", tokens[25].lexeme());
  EXPECT_EQ(TokenType::END_OF_FILE, tokens[26].type());
}

TEST(Scanner, Rewind) {
  {
    const std::string src("hello { while != true");
    ErrorReporter err(src);
    Scanner s(&err, src.begin(), src.end());
    auto tokenOne = s.next();
    EXPECT_EQ(TokenType::IDENTIFIER, tokenOne.type());
    EXPECT_EQ("hello", tokenOne.lexeme());
    auto tokenTwo = s.next();
    EXPECT_EQ(TokenType::LEFT_BRACE, tokenTwo.type());
    EXPECT_EQ("{", tokenTwo.lexeme());
    s.rewind();
    auto tokenThree = s.next();
    EXPECT_EQ(TokenType::LEFT_BRACE, tokenThree.type());
    EXPECT_EQ("{", tokenThree.lexeme());
    auto tokenFour = s.next();
    EXPECT_EQ(TokenType::WHILE, tokenFour.type());
    EXPECT_EQ("while", tokenFour.lexeme());
    auto tokenFive = s.next();
    EXPECT_EQ(TokenType::BANG_EQUAL, tokenFive.type());
    EXPECT_EQ("!=", tokenFive.lexeme());
    auto tokenSix = s.next();
    EXPECT_EQ(TokenType::TRUE, tokenSix.type());
    EXPECT_EQ("true", tokenSix.lexeme());
    auto tokenSeven = s.next();
    EXPECT_EQ(TokenType::END_OF_FILE, tokenSeven.type());
  }
}

}  // namespace lox

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
