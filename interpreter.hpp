#ifndef __INTERPRETER_HPP__
#define __INTERPRETER_HPP__

#include <stack>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <list>

#include "parser.hpp"


struct TreeNode
{
    int data;
    std::list<TreeNode*> children;
};

class Interpreter
{
    public:
        Parser parser;
        int evaluateExpression(std::string& expr);
        void interpretTokens(std::vector<Token> tokens);

    private:
        int getOpPriority(const std::string& op);
        int operation(int& x, int& y, const std::string& op);
        void performLastOp(std::stack<int>& operands, std::stack<std::string>& operators);
        int print(const std::string& str);
};

#endif //__INTERPRETER_HPP__