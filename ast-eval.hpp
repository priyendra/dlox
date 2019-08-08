#include <any>
#include <string>
#include <variant>
#include "ast.hpp"
#include "token.hpp"

namespace lox {
namespace ast {

enum class ValueType { NIL = 0, BOOL = 1, NUMBER = 2, STRING = 3 };

class Value {
 public:
  static Value Nil() {
    Value v;
    v.setNil();
    return v;
  }
  Value() = default;
  Value(bool b) { setBool(b); }
  Value(double d) { setDouble(d); }
  Value(const std::string& s) { setString(s); }
  void setNil() { v_.emplace<0>(); }
  void setBool(bool b) { v_.emplace<1>(b); }
  void setDouble(double d) { v_.emplace<2>(d); }
  void setString(const std::string& s) { v_.emplace<3>(s); }
  ValueType type() const { return static_cast<ValueType>(v_.index()); }
  bool b() const { return std::get<1>(v_); }
  double d() const { return std::get<2>(v_); }
  const std::string& s() const { return std::get<3>(v_); }
  bool equals(const Value& o) const { return v_ == o.v_; }

 private:
  std::variant<std::true_type, bool, double, std::string> v_;
};

class Evaluator {
 public:
  struct Status {
    Status() { ok = true; }
    Status(const std::string& msg, Token t)
      : ok(false), message(msg), token(t) {}
    bool ok;
    std::string message;
    Token token;  // location for the error.
  };
  static Status eval(Node* node, Value* value) {
    try {
      EvalVisitor visitor;
      auto valAny = node->accept(&visitor);
      auto* val = std::any_cast<Value>(&valAny);
      *value = *val;
    } catch (const Status& s) {
      return s;
    }
    return {};
  }

 private:
  struct EvalVisitor : public Visitor {
    static bool isTruthy(Value v) {
      return v.type() != ValueType::NIL &&
             (v.type() != ValueType::BOOL || v.b());
    }
    std::any visitNumber(const Number* obj) override {
      return Value(obj->val);
    }
    std::any visitString(const String* obj) override {
      return Value(obj->val);
    }
    std::any visitBool(const Bool* obj) override {
      return Value(obj->val);
    }
    std::any visitNil(const Nil*) override { return Value::Nil(); }
    std::any visitUnary(const Unary* obj) override {
      auto operandAny = obj->operand->accept(this);
      auto* operand = std::any_cast<Value>(&operandAny);
      switch (obj->op) {
        case Unary::MINUS: {
          if (operand->type() != ValueType::NUMBER) {
            throw Status{"Unary '-' expects numeric argument", obj->opToken};
          }
          return Value(-operand->d());
        }
        case Unary::BANG: {
          return Value(isTruthy(*operand));
        }
      }
      throw Status{"Unexpected operator", obj->opToken};
    }
    std::any visitBinary(const Binary* obj) override {
      auto firstAny = obj->first->accept(this);
      auto* first = std::any_cast<Value>(&firstAny);
      auto secondAny = obj->second->accept(this);
      auto* second = std::any_cast<Value>(&secondAny);
      switch (obj->op) {
        case Binary::MINUS: {
          if (first->type() != ValueType::NUMBER ||
              second->type() != ValueType::NUMBER) {
            throw Status{"Binary '-' expects numeric arguments", obj->opToken};
          }
          return Value(first->d() - second->d());
        }
        case Binary::PLUS: {
          if (first->type() == ValueType::NUMBER &&
              second->type() == ValueType::NUMBER) {
            return Value(first->d() + second->d());
          } else if (first->type() == ValueType::STRING &&
                     second->type() == ValueType::STRING) {
            return Value(first->s() + second->s());
          }
          throw Status{
              "'+' expects both numeric or string arguments", obj->opToken};
        }
        case Binary::SLASH: {
          if (first->type() != ValueType::NUMBER ||
              second->type() != ValueType::NUMBER) {
            throw Status{"'/' expects numeric arguments", obj->opToken};
          }
          return Value(first->d() / second->d());;
        }
        case Binary::STAR: {
          if (first->type() != ValueType::NUMBER ||
              second->type() != ValueType::NUMBER) {
            throw Status{"'*' expects numeric arguments", obj->opToken};
          }
          return Value(first->d() * second->d());;
        }
        case Binary::BANG_EQUAL: {
          return Value(not first->equals(*second));
        }
        case Binary::EQUAL_EQUAL: {
          return Value(first->equals(*second));
        }
        case Binary::GREATER: {
          if (first->type() != ValueType::NUMBER ||
              second->type() != ValueType::NUMBER) {
            throw Status{"'>' expects numeric arguments", obj->opToken};
          }
          return Value(first->d() > second->d());;
        }
        case Binary::GREATER_EQUAL: {
          if (first->type() != ValueType::NUMBER ||
              second->type() != ValueType::NUMBER) {
            throw Status{"'>=' expects numeric arguments", obj->opToken};
          }
          return Value(first->d() >= second->d());;
        }
        case Binary::LESS: {
          if (first->type() != ValueType::NUMBER ||
              second->type() != ValueType::NUMBER) {
            throw Status{"'<' expects numeric arguments", obj->opToken};
          }
          return Value(first->d() < second->d());;
        }
        case Binary::LESS_EQUAL: {
          if (first->type() != ValueType::NUMBER ||
              second->type() != ValueType::NUMBER) {
            throw Status{"'<=' expects numeric arguments", obj->opToken};
          }
          return Value(first->d() <= second->d());;
        }
      }
      return {};
    }
  };
};

}  // namespace ast
}  // namespace lox
