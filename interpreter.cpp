#include "interpreter.h"
#include "error.h"
#include <iostream>

Interpreter::Interpreter(const std::vector<std::shared_ptr<FunctionNode>> &functions)
{
    for (const auto &func : functions)
    {
        functionTable[func->name] = func;
    }
}

ReturnValue Interpreter::run(const std::string &entryPoint)
{
    if (functionTable.find(entryPoint) == functionTable.end())
    {
        std::cerr << "Function '" << entryPoint << "' not found." << std::endl;
        return {};
    }
    return executeFunction(functionTable[entryPoint]);
}

ReturnValue Interpreter::executeFunction(const std::shared_ptr<FunctionNode> &func)
{
    ReturnValue result;

    currentFunctionName = func->name;

    for (const auto &node : func->body)
    {
        auto value = executeNode(node);
        result = value; // update return value if reached
    }

    return result;
}

ReturnValue Interpreter::executeNode(const std::shared_ptr<Node> &node)
{
    switch (node->type)
    {
    case NodeType::Call:
    {
        auto call = std::dynamic_pointer_cast<CallNode>(node);
        if (functionTable.find(call->functionName) != functionTable.end())
        {
            log("Called function " + call->functionName);
            return executeFunction(functionTable[call->functionName]);
        }
        else
        {
            std::cerr << "Function '" << call->functionName << "' not found.\n";
        }
        break;
    }

    case NodeType::Return:
    {
        auto ret = std::dynamic_pointer_cast<ReturnNode>(node);
        ReturnValue val;
        val.s = ret->returnValue;

        if (!val.s.empty() && loggingEnabled)
        {
            std::cout << "Function " << currentFunctionName << " returned "<< val.s << "\n";
        }
        return val;
    }

    case NodeType::Shh:
    {
        auto shh = std::dynamic_pointer_cast<ShhNode>(node);
        loggingEnabled = false;
        auto result = executeNode(shh->inner);
        loggingEnabled = true;
        return result;
    }

    default:
        break;
    }

    return {};
}

void Interpreter::log(const std::string &msg)
{
    if (loggingEnabled)
    {
        std::cout << "[LOG] " << msg << "\n";
    }
}
