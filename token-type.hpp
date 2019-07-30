#pragma once

#include <string>

namespace lox {

// This enum describes all the types of tokens that comprise the Lox language.
enum class TokenType {
  // Simple "punctuation-style" operator tokens
  LEFT_PAREN,    // (
  RIGHT_PAREN,   // )
  LEFT_BRACE,    // {
  RIGHT_BRACE,   // }
  COMMA,         // ,
  DOT,           // .
  MINUS,         // -
  PLUS,          // +
  SEMICOLON,     // ;
  SLASH,         // /
  STAR,          // *
  BANG,          // !
  BANG_EQUAL,    // !=
  EQUAL,         // =
  EQUAL_EQUAL,   // ==
  GREATER,       // >
  GREATER_EQUAL, // >=
  LESS,          // <
  LESS_EQUAL,    // <=
  END_OF_FILE,   // logical token indicating end of file.

  // Reserved keywords
  AND,    // and
  CLASS,  // class
  ELSE,   // else
  EXIT,   // exit
  FALSE,  // false
  FUN,    // fun
  FOR,    // for
  IF,     // if
  NIL,    // nil
  OR,     // or
  PRINT,  // print
  RETURN, // return
  SUPER,  // super
  THIS,   // this
  TRUE,   // true
  VAR,    // var
  WHILE,  // while

  // Tokens that require the original text to fully specify.
  IDENTIFIER, // [a-zA-Z_][a-zA-Z0-9_]*
  STRING,     // ".*"
  NUMBER      // integer/floating point numeric literal
};

// Method to transform a TokenType @t into a human-readable string.
std::string tokenTypeToString(TokenType t);

}  // namespace lox
