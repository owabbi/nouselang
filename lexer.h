#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include "token.h"

class Lexer {
public:
    explicit Lexer(const std::string& source);

    Token nextToken();
    bool hasNext() const;

private:
    std::string source;
    size_t pos;

    char peek() const;
    char advance();
    void skipWhitespace();
    void skipComment();

    Token makeIdentifierOrKeyword();
    Token makeNumber();
    Token makeString();

    bool isAtEnd() const;

    int line = 1;
};

#endif // LEXER_H
