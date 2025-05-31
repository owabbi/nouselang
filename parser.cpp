#include "parser.h"
#include "error.h"
#include <stdexcept>

Parser::Parser(const std::vector<Token> &tokens)
    : tokens(tokens), pos(0) {}

bool Parser::isAtEnd() const
{
    return pos >= tokens.size() || tokens[pos].type == TokenType::EndOfFile;
}

const Token &Parser::peek() const
{
    return tokens[pos];
}

const Token &Parser::advance()
{
    if (!isAtEnd())
        pos++;
    return tokens[pos - 1];
}

bool Parser::match(TokenType type)
{
    if (!isAtEnd() && tokens[pos].type == type)
    {
        advance();
        return true;
    }
    return false;
}

std::vector<std::shared_ptr<FunctionNode>> Parser::parse()
{
    std::vector<std::shared_ptr<FunctionNode>> functions;

    while (!isAtEnd())
    {
        if (peek().type == TokenType::Func)
        {
            functions.push_back(parseFunction());
        }
        else
        {
            throwError("Unexpected token outside of function");
        }
    }

    return functions;
}

std::shared_ptr<FunctionNode> Parser::parseFunction()
{
    match(TokenType::Func); // assume already checked

    if (peek().type != TokenType::Identifier)
        throwError("Expected function name");

    std::string name = advance().value;

    if (!match(TokenType::LBrace))
        throwError("Expected '{' after function name");

    auto func = std::make_shared<FunctionNode>(name);

    while (!match(TokenType::RBrace))
    {
        func->body.push_back(parseStatement());
    }

    return func;
}

std::shared_ptr<Node> Parser::parseStatement()
{
    if (match(TokenType::Shh))
    {
        auto inner = parseStatement(); // recursive
        return std::make_shared<ShhNode>(inner);
    }

    if (match(TokenType::Return))
        return parseReturn();

    if (peek().type == TokenType::Identifier)
        return parseCall();

    throwError("Invalid statement");
}

std::shared_ptr<Node> Parser::parseReturn()
{
    if (!match(TokenType::LParen))
        throwError("Expected '(' after return");

    std::string value;
    if (peek().type != TokenType::RParen)
    {
        Token token = advance();

        switch (token.type)
        {
        case TokenType::StringLiteral:
        case TokenType::BoolLiteral:
        case TokenType::IntLiteral:
            value = token.value;
            break;
        case TokenType::Identifier:
            throwError("Unknown return value: did you forget quotes for a string?");
        default:
            throwError("Invalid return value.");
        }
    }

    if (!match(TokenType::RParen))
        throwError("Expected ')' after return value");

    if(!match(TokenType::Semicolon)) // optional
        throwError("Forgot semi-colon (;)");

    return std::make_shared<ReturnNode>(value);
}

std::shared_ptr<Node> Parser::parseCall()
{
    std::string name = advance().value;
    match(TokenType::Semicolon); // optional

    return std::make_shared<CallNode>(name);
}
