#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>

enum class NodeType {
    Function,
    Call,
    Return,
    Shh
};

struct Node {
    NodeType type;
    virtual ~Node() = default;
};

using NodePtr = std::shared_ptr<Node>;

struct CallNode : public Node {
    std::string functionName;

    CallNode(const std::string& name) : functionName(name) {
        type = NodeType::Call;
    }
};

struct ReturnNode : public Node {
    std::string returnValue; // Keep string, interpret later
    ReturnNode(const std::string& val) : returnValue(val) {
        type = NodeType::Return;
    }
};

struct ShhNode : public Node {
    NodePtr inner;
    ShhNode(NodePtr child) : inner(child) {
        type = NodeType::Shh;
    }
};

struct FunctionNode : public Node {
    std::string name;
    std::vector<NodePtr> body;

    FunctionNode(const std::string& n) : name(n) {
        type = NodeType::Function;
    }
};

#endif // AST_H
