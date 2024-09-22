#pragma once

#include <memory>
#include <any>
#include <optional>

#include "token.h"

class ExprVisitor {
    public:
        virtual std::any visitLiteralExpr(class LiteralExpr*) = 0;
        virtual std::any visitUnaryExpr(class UnaryExpr*) = 0;
        virtual std::any visitBinaryExpr(class BinaryExpr*) = 0;
        virtual std::any visitVariableExpr(class VariableExpr*) = 0;
};

class Expr {
    public:
        virtual std::any accept(ExprVisitor*) = 0;
};

class LiteralExpr : public Expr {
    public:
        LiteralExpr(std::any value)
        : value{value} {}

        std::any accept(ExprVisitor* visitor) override {
            return visitor->visitLiteralExpr(this);
        }
        std::any& getValue() { return value; }
    private:
        std::any value;
};

class UnaryExpr : public Expr {
    public:
        UnaryExpr(Token op, std::unique_ptr<Expr>&& right)
        : op{op}, right{std::move(right)} {}

        std::any accept(ExprVisitor* visitor) override {
            return visitor->visitUnaryExpr(this);
        }
        Token& getOp() { return op; }
        std::unique_ptr<Expr>& getRight() { return right; }
    private:
        Token op;
        std::unique_ptr<Expr> right;
};

class BinaryExpr : public Expr {
    public:
        BinaryExpr(std::unique_ptr<Expr>&& left, Token op, std::unique_ptr<Expr>&& right)
        : left{std::move(left)}, op{op}, right{std::move(right)} {}

        std::any accept(ExprVisitor* visitor) override {
            return visitor->visitBinaryExpr(this);
        }
        std::unique_ptr<Expr>& getLeft() { return left; }
         Token& getOp() { return op; }
        std::unique_ptr<Expr>& getRight() { return right; }
    private:
        std::unique_ptr<Expr> left;
         Token op;
        std::unique_ptr<Expr> right;
};

class StmtVisitor {
    public:
        virtual std::any visitExpressionStmt(class ExpressionStmt*) = 0;
        virtual std::any visitPrintStmt(class PrintStmt*) = 0;
        virtual std::any visitVarStmt(class VarStmt*) = 0;
        virtual std::any visitIfStmt(class IfStmt*) = 0;
        virtual std::any visitBlockStmt(class BlockStmt*) = 0;
};

class Stmt {
    public:
        virtual std::any accept(StmtVisitor*) = 0;
};

class VarStmt : public Stmt {
    public:
        VarStmt(Token name, std::unique_ptr<Expr>&& initializer)
        : name{name}, initializer{std::move(initializer)} {}

        std::any accept(StmtVisitor* visitor) override {
            return visitor->visitVarStmt(this);
        }

        Token& getName() { return name; }
        std::unique_ptr<Expr>& getInitializer() { return initializer; }

    private:
        Token name;
        std::unique_ptr<Expr> initializer;
};

class ExpressionStmt : public Stmt {
    public:
        ExpressionStmt(std::unique_ptr<Expr>&& expression)
        : expression{std::move(expression)} {}

        std::any accept(StmtVisitor* visitor) override {
            return visitor->visitExpressionStmt(this);
        }
        std::unique_ptr<Expr>& getExpression() { return expression; }
    private:
        std::unique_ptr<Expr> expression;
};
class VariableExpr : public Expr {
    public:
        VariableExpr(Token name)
        : name{name} {}

        std::any accept(ExprVisitor* visitor) override {
            return visitor->visitVariableExpr(this);
        }

        Token& getName() { return name; }

    private:
        Token name;
};

class PrintStmt : public Stmt {
    public:
        PrintStmt(std::unique_ptr<Expr>&& expression)
        : expression{std::move(expression)} {}

        std::any accept(StmtVisitor* visitor) override {
            return visitor->visitPrintStmt(this);
        }
        std::unique_ptr<Expr>& getExpression() { return expression; }
    private:
        std::unique_ptr<Expr> expression;
};

class IfStmt : public Stmt {
    public:
        IfStmt(std::unique_ptr<Expr>&& cond, std::unique_ptr<Stmt>&& then) : cond{std::move(cond)}, then{std::move(then)}, other_stmt_given{false} {}

        IfStmt(std::unique_ptr<Expr>&& cond, std::unique_ptr<Stmt>&& then, std::unique_ptr<Stmt>&& otherwise)
        : cond{std::move(cond)}, then{std::move(then)}, otherwise{std::move(otherwise)}, other_stmt_given{true} {}

        std::any accept(StmtVisitor* visitor) override {
            return visitor->visitIfStmt(this);
        }

        std::unique_ptr<Expr>& getCondition() { return cond; }
        std::unique_ptr<Stmt>& getThen() { return then; }
        std::unique_ptr<Stmt>& getOtherwise() { return otherwise; }
        bool hasOtherStmt() const { return other_stmt_given; }
    private:
        std::unique_ptr<Expr> cond;
        std::unique_ptr<Stmt> then;
        std::unique_ptr<Stmt> otherwise;
        bool other_stmt_given;
};

class BlockStmt : public Stmt {
    public:
        // constructor
        BlockStmt(std::vector<std::unique_ptr<Stmt>>&& stmts)
        : stmts{std::move(stmts)}{}
        // accept method
        std::any accept(StmtVisitor* visitor) override {
            return visitor->visitBlockStmt(this);
        }
        // access functions
        std::vector<std::unique_ptr<Stmt>>& getStatements(){
            return stmts;
        }
    private:
        std::vector<std::unique_ptr<Stmt>> stmts;
};