#pragma once

#include <any>
#include <memory>

namespace lox {
namespace ast {

class Visitor;

struct Node {
  virtual ~Node() {}
  virtual std::any accept(Visitor* visitor) const = 0;
};

struct Number : public Node {
  std::any accept(Visitor* visitor) const override;
  double val;
};

struct String : public Node {
  std::any accept(Visitor* visitor) const override;
  std::string val;
};

struct Bool : public Node {
  std::any accept(Visitor* visitor) const override;
  bool val;
};

struct Nil : public Node {
  std::any accept(Visitor* visitor) const override;
};

struct Unary : public Node {
  enum Operator { MINUS, BANG };
  std::any accept(Visitor* visitor) const override;
  Operator op;
  // TODO: If Node is generic AstNode, then this should be Expr. We have not
  // defined a separate Expr class yet because our current goal is to only
  // parse expressions. So expressions are actually the highest point in our
  // AST heirarchy.
  std::unique_ptr<Node> operand;
};

struct Binary : public Node {
  enum Operator {
    MINUS,
    PLUS,
    SLASH,
    STAR,
    BANG_EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,
    AND,
    OR
  };
  std::any accept(Visitor* visitor) const override;
  Operator op;
  std::unique_ptr<Node> first;
  std::unique_ptr<Node> second;
};

class Visitor {
 public:
  virtual ~Visitor() {}
  virtual std::any visitNumber(const Number* obj) = 0;
  virtual std::any visitString(const String* obj) = 0;
  virtual std::any visitBool(const Bool* obj) = 0;
  virtual std::any visitNil(const Nil* nil) = 0;
  virtual std::any visitUnary(const Unary* unary) = 0;
  virtual std::any visitBinary(const Binary* binary) = 0;
};

std::any Number::accept(Visitor* v) const { return v->visitNumber(this); }
std::any String::accept(Visitor* v) const { return v->visitString(this); }
std::any Bool::accept(Visitor* v) const { return v->visitBool(this); }
std::any Nil::accept(Visitor* v) const { return v->visitNil(this); }
std::any Unary::accept(Visitor* v) const { return v->visitUnary(this); }
std::any Binary::accept(Visitor* v) const { return v->visitBinary(this); }

}  // namespace ast
}  // namespace lox
