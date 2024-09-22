#include "lexer.h"

Token Lexer::nextToken() {
    if (peek() == '\0') {
        return Token(TokenType::Eof, "");
    }

    skipWhitespace();

    start = current;

    auto ch = advance();

    switch (ch) {
        case '(': return Token(TokenType::LeftParen, "("); break;
        case ')': return Token(TokenType::RightParen, ")"); break;
        case '{': return Token(TokenType::LeftBrace, "{"); break;
        case '}': return Token(TokenType::RightBrace, "}"); break;
        case ';': return Token(TokenType::Semicolon, ";"); break;
        case ',': return Token(TokenType::Comma, ","); break;
        case '.': return Token(TokenType::Dot, "."); break;
        case '-': return Token(TokenType::Minus, "-"); break;
        case '+': return Token(TokenType::Plus, "+"); break;
        case '*': return Token(TokenType::Star, "*"); break;
        case '=': {
            if (peek() == '=') {
                advance();
                return Token(TokenType::EqualEqual,"==");
            }
            return Token(TokenType::Equal, "=");
        } break;
        case '!': {
            if (peek() == '=') {
                advance();
                return Token(TokenType::BangEqual, "!=");
            }
            return Token(TokenType::Bang, "!");
        } break;
        case '<': {
            if (peek() == '=') {
                advance();
                return Token(TokenType::LessEqual, "<=");
            }
            return Token(TokenType::Less, "<");
        } break;
        case '>': {
            if (peek() == '=') {
                advance();
                return Token(TokenType::GreaterEqual, ">=");
            }
            return Token(TokenType::Greater, ">");
        } break;
        case '/': {
            if (peek() == '/') {
                while (peek() != '\n' && peek() != '\0') {
                    advance();
                }
                return nextToken();
            }
            return Token(TokenType::Slash, "/");
        } break;
        case '"': { return string(); } break;
        default: {
            if (isdigit(ch)) {
                return number();
            } else if (isalpha(ch)) {
                return identifier();
            } else {
                return Token(TokenType::Unknown, std::string(1, ch));
            }
        }
    }
}

std::vector<Token> Lexer::getTokens() {
    std::vector<Token> tokens;
    while (true) {
        auto token = nextToken();
        if (token.type == TokenType::Eof) {
            break;
        }
        tokens.push_back(token);
    }
    return tokens;
}

void Lexer::skipWhitespace() {
    while (true) {
        auto ch = peek();
        switch (ch) {
            case ' ':
            case '\t':
            case '\r': {
                advance();
            } break;
            case '\n': {
                line++;
                advance();
            } break;
            default: {
                return;
            } break;
        }
    }
}

void Lexer::skipComment() {
    while (true) {
        auto ch = peek();
        switch (ch) {
            case '\n': {
                line++;
                advance();
                return;
            } break;
            default: {
                advance();
            } break;
        }
    }
}

char Lexer::peek_next() {
    if (current + 1 >= input.size()) {
        return '\0';
    }
    return input[current + 1];
}

char Lexer::peek() {
    if (current >= input.size()) {
        return '\0';
    }
    return input[current];
}

char Lexer::advance() {
    current++;
    return input[current - 1];
}

Token Lexer::identifier() {
    while (isalpha(peek()) || isdigit(peek())) {
        advance();
    }
    std::string str = input.substr(start, current - start);

    switch (str[0]) {
        case 'a': {
            if (str == "and") {
                return Token(TokenType::And, str);
            }
        }; break;
        case 'b': {
            if (str == "break") {
                return Token(TokenType::Break, str);
            }
        }; break;
        case 'c': {
            if (str == "class") {
                return Token(TokenType::Class, str);
            } else if (str == "continue") {
                return Token(TokenType::Continue, str);
            }
        }; break;
        case 'd': {
            if (str == "do") {
                return Token(TokenType::Do, str);
            }
        }; break;
        case 'e': {
            if (str == "else") {
                return Token(TokenType::Else, str);
            } else if (str == "elseif") {
                return Token(TokenType::ElseIf, str);
            }
        }; break;
        case 'f': {
            if (str == "false") {
                return Token(TokenType::False, str);
            } else if (str == "for") {
                return Token(TokenType::For, str);
            } else if (str == "fun") {
                return Token(TokenType::Fun, str);
            }
        }; break;
        case 'i': {
            if (str == "if") {
                return Token(TokenType::If, str);
            } else if (str == "in") {
                return Token(TokenType::In, str);
            }
        }; break;
        case 'l': {
            if (str == "let") {
                return Token(TokenType::Let, str);
            }
        }; break;
        case 'n': {
            if (str == "nil") {
                return Token(TokenType::Nil, str);
            }
        }; break;
        case 'o': {
            if (str == "or") {
                return Token(TokenType::Or, str);
            }
        }; break;
        case 'p': {
            if (str == "print") {
                return Token(TokenType::Print, str);
            }
        }; break;
        case 'r': {
            if (str == "return") {
                return Token(TokenType::Return, str);
            }
        }; break;
        case 's': {
            if (str == "super") {
                return Token(TokenType::Super, str);
            } else if (str == "static") {
                return Token(TokenType::Static, str);
            } else if (str == "struct") {
                return Token(TokenType::Struct, str);
            } else if (str == "switch") {
                return Token(TokenType::Switch, str);
            }
        }; break;
        case 't': {
            if (str == "true") {
                return Token(TokenType::True, str);
            } else if (str == "this") {
                return Token(TokenType::This, str);
            }
        }; break;
        case 'v': {
            if (str == "var") {
                return Token(TokenType::Var, str);
            }
        }; break;
        case 'w': {
            if (str == "while") {
                return Token(TokenType::While, str);
            }
        }; break;
    }
    return Token(TokenType::Identifier, str);
}

Token Lexer::number() {
    while (isdigit(peek())) {
        advance();
    }
    
    if (peek() == '.' && isdigit(peek_next())) {
        advance();
        while (isdigit(peek())) {
            advance();
        }
    }

    std::string str = input.substr(start, current - start);
    return Token(TokenType::Number, str, std::stof(str) );
}

Token Lexer::string() {
    while (peek() != '"' && peek() != '\0') {
        if (peek() == '\n') {
            line++;
        }
        advance();
    }
    if (peek() == '\0') {
        return Token(TokenType::Unknown, "Unterminated string");
    }
    advance();
    std::string str = input.substr(start + 1, current - start - 2);
    return Token(TokenType::String, str, str);
}
