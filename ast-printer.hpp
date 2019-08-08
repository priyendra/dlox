#include <any>
#include <fmt/format.h>
#include <sstream>
#include "ast.hpp"

namespace lox {
namespace ast {

class Printer {
 public:
  static std::string print(Node* node, bool multiLine = false) {
    PrintVisitor visitor(multiLine);
    auto printedAny = node->accept(&visitor);
    auto* printed = std::any_cast<Serialized>(&printedAny);
    return printed->s;
  }

 private:
  struct Serialized {
    std::string s;
    bool terminal;
  };
  struct PrintVisitor : public Visitor {
    PrintVisitor(bool multiLine = false) : multiLine_(multiLine) {}
    std::any visitNumber(const Number* obj) override {
      return Serialized{fmt::format("{}", obj->val), true};
    }
    std::any visitString(const String* obj) override {
      return Serialized{fmt::format("'{}'", obj->val), true};
    }
    std::any visitBool(const Bool* obj) override {
      const char* val = obj->val ? "true" : "false";
      return Serialized{fmt::format("{}", val), true};
    }
    std::any visitNil(const Nil*) override { return Serialized{"nil", true}; }
    std::any visitUnary(const Unary* obj) override {
      indent_ += 2;
      auto operandAny = obj->operand->accept(this);
      indent_ -= 2;
      auto* operand = std::any_cast<Serialized>(&operandAny);
      const char* op = "";
      switch (obj->op) {
        case Unary::MINUS:
          op = "-";
          break;
        case Unary::BANG:
          op = "!";
          break;
      }
      std::stringstream ss;
      if (not multiLine_ || operand->terminal) {
        ss << "(" << op << " " << operand->s << ")";
      } else {
        ss << "(" << op << "\n"
           << indent() << "  " << operand->s << "\n"
           << indent() << ")";
      }
      return Serialized{ss.str(), false};
    }
    std::any visitBinary(const Binary* obj) override {
      indent_ += 2;
      auto firstAny = obj->first->accept(this);
      auto secondAny = obj->second->accept(this);
      indent_ -= 2;
      auto* first = std::any_cast<Serialized>(&firstAny);
      auto* second = std::any_cast<Serialized>(&secondAny);
      const char* op = "";
      switch (obj->op) {
        case Binary::MINUS:
          op = "-";
          break;
        case Binary::PLUS:
          op = "+";
          break;
        case Binary::SLASH:
          op = "/";
          break;
        case Binary::STAR:
          op = "*";
          break;
        case Binary::BANG_EQUAL:
          op = "!=";
          break;
        case Binary::EQUAL_EQUAL:
          op = "==";
          break;
        case Binary::GREATER:
          op = ">";
          break;
        case Binary::GREATER_EQUAL:
          op = ">=";
          break;
        case Binary::LESS:
          op = "<";
          break;
        case Binary::LESS_EQUAL:
          op = "<=";
          break;
      }
      std::stringstream ss;
      if (not multiLine_ || (first->terminal && second->terminal)) {
        ss << "(" << op << " " << first->s << ", " << second->s << ")";
      } else {
        ss << "(" << op << "\n"
           << indent() << "  " << first->s << ",\n"
           << indent() << "  " << second->s << "\n"
           << indent() << ")";
      }
      return Serialized{ss.str(), false};
    }
    std::string indent() const { return std::string(indent_, ' '); }
    bool multiLine_ = false;
    int indent_ = 0;
  };
};

}  // namespace ast
}  // namespace lox
