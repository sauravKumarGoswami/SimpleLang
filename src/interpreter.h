#include "expr.h"
#include "token.h"
#include <iostream>
#include <memory>
#include <vector>
#include <any>

class Interpreter: public ExprVisitor, public StmtVisitor {
    public:
        void interpret(std::vector<std::unique_ptr<Stmt>>& statements) {
            for (auto& stmt: statements) {
                execute(stmt);
            }
        }
    private:
        void execute(std::unique_ptr<Stmt>& stmt) {
            stmt->accept(this);
        }

        std::any evaluate(std::unique_ptr<Expr>& expr) {
            return expr->accept(this);
        }

        std::any visitLiteralExpr(LiteralExpr* expr) override {
            return expr->getValue();
        }

        std::any visitUnaryExpr(UnaryExpr* expr) override {
            std::any ret = expr->getRight()->accept(this);
            switch (expr->getOp().type) {
                case TokenType::Minus: return -std::any_cast<float>(ret); break;
                default: break;
            }
            return ret;
        }

        std::any visitBinaryExpr(BinaryExpr* expr) override {
            float left = std::any_cast<float>(expr->getLeft()->accept(this));
            float right = std::any_cast<float>(expr->getRight()->accept(this));
            float ret = 0;
            switch (expr->getOp().type) {
                case TokenType::Plus: ret = left + right; break;
                case TokenType::Minus: ret =  left - right; break;
                case TokenType::Star: ret = left * right; break;
                case TokenType::Slash: {
                    if (right == 0)
                        throw std::runtime_error("right operand is 0");
                    ret = left / right;
                } break;
                case TokenType::Less: ret = (left < right); break;
                case TokenType::LessEqual: ret = (left <= right); break;
                case TokenType::Greater: ret = (left > right); break;
                case TokenType::GreaterEqual: ret = (left >= right); break;
                case TokenType::EqualEqual: ret = (left == right); break;
                default: break;
            }
            return ret;
        }

        std::any visitExpressionStmt(ExpressionStmt* stmt) override {
            evaluate(stmt->getExpression());
            return nullptr;
        }

        std::any visitPrintStmt(PrintStmt* stmt) override {
            std::any value = evaluate(stmt->getExpression());
            if (value.type() == typeid(float)) {
                std::cout << std::any_cast<float>(value);
            }
            if (value.type() == typeid(std::string)) {
                std::cout << std::any_cast<std::string>(value);
            }
            return nullptr;
        }

        std::any visitVarStmt(VarStmt*) override {}
        std::any visitVariableExpr(VariableExpr*) override {}

        std::any visitBlockStmt(BlockStmt* stmt) override {
            for (auto& s: stmt->getStatements()) {
                s->accept(this);
            }
            return nullptr;
        }

        std::any visitIfStmt(IfStmt* stmt) override {
            auto condition = std::any_cast<float>(evaluate(stmt->getCondition()));
            if (condition == 1.0) {
                execute(stmt->getThen());
            } else {
                if (stmt->hasOtherStmt()) {
                    execute(stmt->getOtherwise());
                }
            }
            return nullptr;
        }
};