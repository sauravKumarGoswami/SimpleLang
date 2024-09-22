#include <iostream>
#include <fstream>
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

void eval(const std::string& code) {
    Lexer lexer{code};
    auto tokens = lexer.getTokens();

    Parser parser{tokens};

    try {
        auto statements = parser.parse();
        Interpreter interpreter;
        interpreter.interpret(statements);
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void repl() {

    std::string code;

    while (true) {
        std::cout << ">> ";
        std::getline(std::cin, code);
        eval(code);
    }
}

void read(const std::string& path) {

    std::string code;

    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << path << std::endl;
        return;
    }

    std::string line;
    while (getline(file, line)) {
        code += line;
    }
    file.close();

    eval(code);
}

int main(int argc, char** argv) {

    switch (argc) {
        case 1: repl(); break;
        case 2: read(argv[1]); break;
        default: return -1;
    }

    return 0;
}
