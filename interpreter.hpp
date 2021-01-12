#ifndef __INTERPRETER_HPP__
#define __INTERPRETER_HPP__

#include <stack>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <list>

#include "parser.hpp"
#include "varhandler.hpp"


struct TreeNode
{
    int data;
    std::list<TreeNode*> children;
};



class Interpreter
{
    public:
        Parser parser;
        int evaluateExpression(const std::string& expr);
        void interpretTokens(std::vector<Token> tokens);

    private:
        int getOpPriority(const std::string& op);
        int operation(int& x, int& y, const std::string& op);
        void performLastOp(std::stack<int>& operands, std::stack<std::string>& operators);
        //
        void _let(const std::string& str);
        void _read(const std::string& str);
        void _print(const std::string& str);
        void _goto(const std::string& str);
        void _label(const std::string& str);
        void _while(const std::string& str);
        void _done(const std::string& str);
        void _if(const std::string& str);
        void _else(const std::string& str);
        void _endif(const std::string& str);
        void _assign(const std::string& str);
};

#endif //__INTERPRETER_HPP__