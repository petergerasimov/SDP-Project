#ifndef __INTERPRETER_HPP__
#define __INTERPRETER_HPP__

#include <stack>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <list>

#include "parser.hpp"
#include "varhandler.hpp"
#include "expressiontree.hpp"

class Interpreter
{
    public:
        Parser parser;
        ExpressionTree expTreeHelper;
        int evaluateTree(ExpressionTree::Node* tree);
        int evaluateExpression(const std::string& expr);
        void interpretTokens(std::vector<Token> tokens);

    private:
        VarHandler varHandler;
        int getOpPriority(const std::string& op);
        int operation(int x, int y, const std::string& op);
        void performLastOp(std::stack<int>& operands, std::stack<std::string>& operators);
        //
        int _let(const std::string& str);
        int _read(const std::string& str);
        int _print(const std::string& str);
        int _while(const std::string& str);
        int _done(const std::string& str);
        int _if(const std::string& str);
        int _else(const std::string& str);
        int _endif(const std::string& str);
        int _assign(const std::string& str);
};

#endif //__INTERPRETER_HPP__