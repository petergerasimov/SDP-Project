#include "interpreter.hpp"

// TODO
// Make a parser for expressions first
// lecture from 7.12
// that's done with 2 stacks

// https://dyingdown.github.io/2019/11/12/Calculator/
// The pictures from this look like the lecture

#include <stack>
#include <cstring>
#include <stdexcept>
#include <iostream>


int Interpreter::evaluateExpression(std::string& expr)
{
    std::stack<int> operands;
    std::stack<std::string> operators;

    std::vector<Token> tokens = parser.parseExpression(expr);

    for(const Token& t : tokens)
    {
        std::cout << t.data << std::endl;
        if(t.keywrd == INT)
        {
            operands.push(atoi(t.data.c_str()));
        }
        else if (t.keywrd == OPERATOR)
        {
            if(!operators.empty())
            {
                if(getOpPriority(t.data) <= getOpPriority(operators.top()))
                {
                    int x = operands.top();
                    operands.pop();
                    int y = operands.top();
                    operands.pop();
                    operands.push(operation(y, x, operators.top()));
                    operators.pop();
                }
            }
            operators.push(t.data);
        }
    }
    while(!operators.empty())
    {
        int x = operands.top();
        operands.pop();
        int y = operands.top();
        operands.pop();
        operands.push(operation(y, x, operators.top()));
        operators.pop();
    }
    return operands.top();
}

// this can be done better with a hash function or std::map
// И оператори =, ==, !=, <, <=, >, >=, +, *, /, -, %, &&, ||, !,()
// https://en.cppreference.com/w/cpp/language/operator_precedence
int Interpreter::getOpPriority(const std::string& op)
{
    if(!op.compare("("))
    {
        return 0;
    }
    else if(!op.compare("="))
    {
        return 1;
    }
    else if(!op.compare("||"))
    {
        return 2;
    }
    else if(!op.compare("&&"))
    {
        return 3;
    }
    else if(!op.compare("==") || !op.compare("!="))
    {
        return 4;
    }
    else if(!op.compare("<") || !op.compare("<=") ||
            !op.compare(">") || !op.compare(">="))
    {
        return 5;
    }
    else if(!op.compare("+") || !op.compare("-"))
    {
        return 6;
    }
    else if(!op.compare("*") || !op.compare("/") ||
            !op.compare("%"))
    {
        return 7;
    }
    else if(!op.compare("!"))
    {
        return 8;
    }
    else if(!op.compare(")"))
    {
        return 9;
    }
    else
    {
        throw std::runtime_error("Invalid operator");
    }
    
}

int Interpreter::operation(int& x, int& y, const std::string& op)
{
    if(!op.compare("||"))
    {
        return x || y;
    }
    else if(!op.compare("&&"))
    {
        return x && y;
    }
    else if(!op.compare("==") || !op.compare("!="))
    {
        return x == y;
    }
    else if(!op.compare("<"))
    {
        return x < y;
    }
    else if(!op.compare("<="))
    {
        return x <= y;
    }
    else if(!op.compare(">"))
    {
        return x > y;
    }
    else if(!op.compare(">="))
    {
        return x >= y;
    }
    else if(!op.compare("+"))
    {
        return x + y;
    }
    else if(!op.compare("-"))
    {
        return x - y;
    }
    else if(!op.compare("*"))
    {
        return x * y;
    }
    else if(!op.compare("/"))
    {
        return x / y;
    }
    else if(!op.compare("%"))
    {
        return x % y;
    }
    else
    {
        throw std::runtime_error("Invalid operator");
    }
    
}