#pragma once

#include <memory>
#include <vector>
#include "token.h"
#include <iostream>
#include "expr.h"

class Parser {
    public:
        Parser(const std::vector<Token>& tokens)
        : tokens{tokens} {}

        std::vector<std::unique_ptr<Stmt>> parse();

    private:
        bool atEnd();
        Token peek();
        Token advance();
        Token previous();
        bool eat(TokenType type);
        void consume(TokenType type, const std::string& err);
        bool check(TokenType type);
        bool match(TokenType type);

        std::unique_ptr<Stmt> statement();
        std::unique_ptr<Stmt> blockStmt();
        std::unique_ptr<Stmt> expressionStatement();
        std::unique_ptr<Stmt> printStatement();
        std::unique_ptr<Stmt> IfStatement();
        std::unique_ptr<Expr> expr();
        std::unique_ptr<Expr> equality();
        std::unique_ptr<Expr> comparison();
        std::unique_ptr<Expr> term();
        std::unique_ptr<Expr> factor();
        std::unique_ptr<Expr> unary();
        std::unique_ptr<Expr> primary(); 

    private:
        std::vector<Token> tokens;
        size_t current = 0;
};
