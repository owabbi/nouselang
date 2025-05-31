#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

struct ReturnValue {
    int n = 0;
    bool b = true;
    std::string s = "";
};

class Interpreter {
public:
    Interpreter(const std::vector<std::shared_ptr<FunctionNode>>& functions);
    ReturnValue run(const std::string& entryPoint);

private:
    std::unordered_map<std::string, std::shared_ptr<FunctionNode>> functionTable;
    bool loggingEnabled = true;

    ReturnValue executeFunction(const std::shared_ptr<FunctionNode>& func);
    ReturnValue executeNode(const std::shared_ptr<Node>& node);
    void log(const std::string& msg);

    std::string currentFunctionName;
};

#endif // INTERPRETER_H
