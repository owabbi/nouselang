#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "ast.h"
#include <vector>
#include <memory>

class Parser {
public:
    Parser(const std::vector<Token>& tokens);

    std::vector<std::shared_ptr<FunctionNode>> parse();

private:
    std::vector<Token> tokens;
    size_t pos;

    const Token& peek() const;
    const Token& advance();
    bool match(TokenType type);
    bool isAtEnd() const;

    std::shared_ptr<FunctionNode> parseFunction();
    std::shared_ptr<Node> parseStatement();
    std::shared_ptr<Node> parseReturn();
    std::shared_ptr<Node> parseCall();
    std::shared_ptr<Node> parseShh();
};

#endif // PARSER_H
