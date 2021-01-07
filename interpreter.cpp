#include "interpreter.hpp"

// TODO
// Make a parser for expressions first
// lecture from 7.12
// that's done with 2 stacks

// https://dyingdown.github.io/2019/11/12/Calculator/
// The pictures from this look like the lecture

#include <stack>
#include <cstring>


int Interpreter::evaluateExpression(std::string expr)
{
    std::stack<int> operands;
    std::stack<char*> operators;

    // ! to / and  < to  >
    
}

// this can be done better with a hash function or std::map
// И оператори =, ==, !=, <, <=, >, >=, +, *, /, -, %, &&, ||, !,()
// https://en.cppreference.com/w/cpp/language/operator_precedence
int Interpreter::getOpPriority(char op[3])
{
    if(!strcmp(op, "("))
    {
        return 0;
    }
    else if(!strcmp(op, "="))
    {
        return 1;
    }
    else if(!strcmp(op, "||"))
    {
        return 2;
    }
    else if(!strcmp(op, "&&"))
    {
        return 3;
    }
    else if(!strcmp(op, "==") || !strcmp(op, "!="))
    {
        return 4;
    }
    else if(!strcmp(op, "<") || !strcmp(op, "<=") ||
            !strcmp(op, ">") || !strcmp(op, ">="))
    {
        return 5;
    }
    else if(!strcmp(op, "+") || !strcmp(op, "-"))
    {
        return 6;
    }
    else if(!strcmp(op, "*") || !strcmp(op, "/") ||
            !strcmp(op, "%"))
    {
        return 7;
    }
    else if(!strcmp(op, "!"))
    {
        return 8;
    }
    else if(!strcmp(op, ")"))
    {
        return 9;
    }
    else
    {
        return -1;
    }
    
}