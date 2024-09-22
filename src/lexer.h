#pragma once

#include "token.h"
#include <string>
#include <vector>

class Lexer {
    public:
        std::string input;
        size_t start;
        size_t current;
        size_t line;

    public:
        Lexer(const std::string& input) : input(input), start(0), current(0), line(1) {}

        Token nextToken();
        std::vector<Token> getTokens();

    private:

        void skipWhitespace();
        void skipComment();
        char peek_next();
        char peek();
        char advance();

        Token identifier();
        Token number();
        Token string();
};
