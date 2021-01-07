#ifndef __INTERPRETER_HPP__
#define __INTERPRETER_HPP__

#include "parser.hpp"

class Interpreter
{
    public:
        Parser parser;
        int evaluateExpression(std::string& expr);
    private:
        int getOpPriority(const std::string& op);
        int operation(int& x, int& y, const std::string& op);
};

#endif //__INTERPRETER_HPP__