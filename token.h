#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    Func,
    Return,
    Shh,

    Identifier,
    IntLiteral,
    BoolLiteral,
    StringLiteral,

    LParen,     // (
    RParen,     // )
    LBrace,     // {
    RBrace,     // }
    Comma,      // ,
    Semicolon,  // ;

    EndOfFile
};

struct Token {
    TokenType type;
    std::string value;
};

inline std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::Func: return "Func";
        case TokenType::Identifier: return "Identifier";
        case TokenType::Return: return "Return";
        case TokenType::Shh: return "Shh";
        case TokenType::StringLiteral: return "StringLiteral";
        case TokenType::BoolLiteral: return "BoolLiteral";
        case TokenType::IntLiteral: return "IntLiteral";
        case TokenType::LBrace: return "LBrace";
        case TokenType::RBrace: return "RBrace";
        case TokenType::LParen: return "LParen";
        case TokenType::RParen: return "RParen";
        case TokenType::Semicolon: return "Semicolon";
        case TokenType::EndOfFile: return "EndOfFile";
        default: return "Unknown";
    }
}


#endif // TOKEN_H
