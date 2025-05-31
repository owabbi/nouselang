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
        return Token{TokenType::EndOfFile, "", line};

    char c = advance();

    // Symbols
    switch (c)
    {
    case '(':
        return Token{TokenType::LParen, "(", line};
    case ')':
        return Token{TokenType::RParen, ")", line};
    case '{':
        return Token{TokenType::LBrace, "{", line};
    case '}':
        return Token{TokenType::RBrace, "}", line};
    case ',':
        return Token{TokenType::Comma, ",", line};
    case ';':
        return Token{TokenType::Semicolon, ";", line};
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

    return Token{TokenType::EndOfFile, "", line}; // fallback
}

Token Lexer::makeIdentifierOrKeyword()
{
    std::string value;
    while (!isAtEnd() && (std::isalnum(peek()) || peek() == '_'))
    {
        value += advance();
    }

    if (value == "func")
        return Token{TokenType::Func, value, line};
    if (value == "return")
        return Token{TokenType::Return, value, line};
    if (value == "shh")
        return Token{TokenType::Shh, value};
    if (value == "true" || value == "false")
        return Token{TokenType::BoolLiteral, value, line};

    return Token{TokenType::Identifier, value, line};
}

Token Lexer::makeNumber()
{
    std::string value;
    while (!isAtEnd() && std::isdigit(peek()))
    {
        value += advance();
    }
    return Token{TokenType::IntLiteral, value, line};
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
        throwError("Missing closing quote (\") for string literal", line);
    }

    advance(); // skip closing quote
    return Token{TokenType::StringLiteral, value, line};
}
