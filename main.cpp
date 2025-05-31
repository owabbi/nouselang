#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "interpreter.h"
#include "ast.h"

std::string readFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Erreur : impossible d’ouvrir le fichier " << filename << std::endl;
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void printNode(const std::shared_ptr<Node> &node, int indent = 2)
{
    std::string prefix(indent, ' ');
    switch (node->type)
    {
    case NodeType::Call:
        std::cout << prefix << "Call: " << std::dynamic_pointer_cast<CallNode>(node)->functionName << "\n";
        break;
    case NodeType::Return:
        std::cout << prefix << "Return: " << std::dynamic_pointer_cast<ReturnNode>(node)->returnValue << "\n";
        break;
    case NodeType::Shh:
        std::cout << prefix << "Shh:\n";
        printNode(std::dynamic_pointer_cast<ShhNode>(node)->inner, indent + 2);
        break;
    default:
        std::cout << prefix << "Unknown node\n";
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage : ./nouselang <fichier.nouse>" << std::endl;
        return 1;
    }

    std::string source = readFile(argv[1]);
    Lexer lexer(source);
    std::vector<Token> tokens;
    while (lexer.hasNext())
    {
        tokens.push_back(lexer.nextToken());
    }

    Parser parser(tokens);
    auto functions = parser.parse();

    std::string mode = argc > 2 ? argv[2] : "";

    if (mode == "-tokens")
    {
        for (const auto &token : tokens)
        {
            std::cout << "Token: " << tokenTypeToString(token.type)
                      << " | Value: \"" << token.value << "\"\n";
        }
        return 0;
    }

    if (mode == "-ast")
    {
        std::cout << "=== AST ===\n";
        for (const auto &func : functions)
        {
            std::cout << "Function: " << func->name << "\n";
            for (const auto &node : func->body)
            {
                printNode(node, 2);
            }
        }
        return 0;
    }

    Interpreter interpreter(functions);
    interpreter.run("main");

    return 0;
}
