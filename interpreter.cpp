#include "interpreter.hpp"

// TODO
// Make a parser for expressions first
// lecture from 7.12
// that's done with 2 stacks

// https://dyingdown.github.io/2019/11/12/Calculator/
// The pictures from this look like the lecture

int Interpreter::evaluateExpression(const std::string& expr)
{
    std::stack<int> operands;
    std::stack<std::string> operators;

    std::vector<Token> tokens = parser.parseExpression(expr);

    for(const Token& t : tokens)
    {
       
        if(t.keywrd == INT)
        {
            operands.push(atoi(t.data.c_str()));
        }
        else if (t.keywrd == OPERATOR)
        {
            if(!operators.empty())
            {
                if(!t.data.compare(")"))
                {
                    //TODO: check if there are the right amount of brackets
                    while(operators.top().compare("("))
                    {
                        performLastOp(operands, operators);
                    }
                    operators.pop();
                }
                else if(getOpPriority(t.data) <= getOpPriority(operators.top()))
                {
                    if(operators.top().compare("("))
                    {
                        performLastOp(operands, operators);
                    }
                }
            }
            if(t.data.compare(")"))
            {
                operators.push(t.data);
            }
        }
    }
    while(!operators.empty())
    {
        performLastOp(operands, operators);
    }
    return operands.top();
}


void Interpreter::performLastOp(std::stack<int>& operands, std::stack<std::string>& operators)
{
    if(operands.size() < 2)
    {
        return;
    }
    int x = operands.top();
    operands.pop();
    int y = operands.top();
    operands.pop();
    operands.push(operation(y, x, operators.top()));
    operators.pop();
}
//:))) in class function pointer ;))))))))
typedef int (Interpreter::*func)(const std::string& str);
int Interpreter::_let(const std::string& str)
{
    //TODO add arrays
    varHandler.add(str);
    return 1;
}
int Interpreter::_read(const std::string& str)
{
    //TODO make it work with arrays
    int tmp;
    std::cin >> tmp;
    varHandler.changeValue(str, tmp);
    return 1;
}
int Interpreter::_print(const std::string& str)
{
    //std::cout << evaluateExpression(str);
    std::cout << varHandler.getValue(str);
    std::cout << std::endl;
    return 1;
}
int Interpreter::_while(const std::string& str)
{
    std::cout << str;
    return 1;
}
int Interpreter::_done(const std::string& str)
{
    std::cout << str;
    return 1;
}
int Interpreter::_if(const std::string& str)
{
    std::cout << str;
    return 1;
}
int Interpreter::_else(const std::string& str)
{
    std::cout << str;
    return 1;
}
int Interpreter::_endif(const std::string& str)
{
    std::cout << str;
    return 1;
}
int Interpreter::_assign(const std::string& str)
{
    //TODO make it work with arrays
    //Also redesign wtih new expression evaluator
    std::string buffVar;
    std::string buffExpr;
    bool foundEq = false;
    for(const char c : str)
    {
        if(c == '=')
        {
            foundEq = true;
            continue;
        }
        if(!foundEq)
        {
            buffVar.push_back(c);
        }
        else
        {
            buffExpr.push_back(c);
        }
        
    }
    int exprValue = evaluateExpression(buffExpr);
    varHandler.changeValue(buffVar, exprValue);
    return 1;
}

void Interpreter::interpretTokens(std::vector<Token> tokens)
{
    // enum keyWord {LET, READ, PRINT, WHILE, DONE, IF, ELSE, ENDIF, ASSIGN, GOTO, LABEL, INT, OPERATOR};;
    // INT and OPERATOR are for expressions only
    static const size_t numOfFuncs = 9;
    static const func functions[numOfFuncs] = {_let, _read, _print, _while, _done, _if, 
                                               _else, _endif, _assign};
    
    int sz = tokens.size();
    for(size_t i = 0; i < sz; i++)
    {
        if(tokens[i].keywrd == GOTO)
        {
            for(size_t j = 0; j < sz; j++)
            {
                if(tokens[j].keywrd == LABEL)
                {
                    if(tokens[j].data == tokens[i].data)
                    {
                        i = --j;
                        continue;
                    }
                }
            }
        }
        else if(tokens[i].keywrd < numOfFuncs)
        {
            (this->*functions[tokens[i].keywrd])(tokens[i].data);
        }
    }
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
        throw std::runtime_error("Invalid operator " + op);
        return -1;
    }
    
}

//TODO MAKE THIS BETTER
//EITHER WITH MAP OR UNORDERED_MAP
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
        throw std::runtime_error("Invalid operator " + op);
        return -1;
    }
    
}