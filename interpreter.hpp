#ifndef __INTERPRETER_HPP__
#define __INTERPRETER_HPP__

#include "parser.hpp"

class Interpreter
{
    public:
        Parser parser;
        int evaluateExpression(std::string expr);
    private:
        int getOpPriority(std::string op);
};

#endif //__INTERPRETER_HPP__