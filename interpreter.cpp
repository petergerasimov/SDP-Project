#include "interpreter.hpp"

// TODO
// Make a parser for expressions first
// lecture from 7.12
// that's done with 2 stacks

// https://dyingdown.github.io/2019/11/12/Calculator/
// The pictures from this look like the lecture


int Interpreter::evaluateExpression(std::string expr)
{
    
    
}

// this can be done better with a hash function or std::map
// И оператори =, ==, !=, <, <=, >, >=, +, *, /, -, %, &&, ||, !,()
// https://en.cppreference.com/w/cpp/language/operator_precedence
int Interpreter::getOpPriority(std::string op)
{
    if(!op.compare("="))
    {
        return 1;
    }
}