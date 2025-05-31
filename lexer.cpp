#include "lexer.h"
#include "error.h"
#include <cctype>

Lexer::Lexer(const std::string &src) : source(src), pos(0) {}

bool Lexer::hasNext() const
{
    return !isAtEnd();
}

char Lexer::peek() const
{
    return isAtEnd() ? '\0' : source[pos];
}

char Lexer::advance()
{
    if (isAtEnd()) return '\0';

    char c = source[pos++];
    if (c == '\n') line++;
    return c;
}

bool Lexer::isAtEnd() const
{
    return pos >= source.length();
}

void Lexer::skipWhitespace()
{
    while (!isAtEnd() && std::isspace(peek()))
    {
        advance();
    }
}

void Lexer::skipComment()
{
    if (peek() == '/' && pos + 1 < source.length() && source[pos + 1] == '/')
    {
        while (!isAtEnd() && peek() != '\n')
        {
            advance();
        }
    }
}

Token Lexer::nextToken()
{
    skipWhitespace();
    skipComment();
    skipWhitespace();

    if (isAtEnd())
        return Token{TokenType::EndOfFile, ""};

    char c = advance();

    // Symbols
    switch (c)
    {
    case '(':
        return Token{TokenType::LParen, "("};
    case ')':
        return Token{TokenType::RParen, ")"};
    case '{':
        return Token{TokenType::LBrace, "{"};
    case '}':
        return Token{TokenType::RBrace, "}"};
    case ',':
        return Token{TokenType::Comma, ","};
    case ';':
        return Token{TokenType::Semicolon, ";"};
    case '"':
        return makeString();
    }

    // Number literal
    if (std::isdigit(c))
    {
        pos--; // put back for full parsing
        return makeNumber();
    }

    // Identifier or keyword
    if (std::isalpha(c))
    {
        pos--; // put back for full parsing
        return makeIdentifierOrKeyword();
    }

    return Token{TokenType::EndOfFile, ""}; // fallback
}

Token Lexer::makeIdentifierOrKeyword()
{
    std::string value;
    while (!isAtEnd() && (std::isalnum(peek()) || peek() == '_'))
    {
        value += advance();
    }

    if (value == "func")
        return Token{TokenType::Func, value};
    if (value == "return")
        return Token{TokenType::Return, value};
    if (value == "shh")
        return Token{TokenType::Shh, value};
    if (value == "true" || value == "false")
        return Token{TokenType::BoolLiteral, value};

    return Token{TokenType::Identifier, value};
}

Token Lexer::makeNumber()
{
    std::string value;
    while (!isAtEnd() && std::isdigit(peek()))
    {
        value += advance();
    }
    return Token{TokenType::IntLiteral, value};
}

Token Lexer::makeString()
{
    std::string value;
    while (!isAtEnd() && peek() != '"')
    {
        value += advance();
    }

    if (isAtEnd())
    {
        throwError("Missing closing quote (\") for string literal");
    }

    advance(); // skip closing quote
    return Token{TokenType::StringLiteral, value};
}
