#include "interpreter.hpp"

// TODO
// Make a parser for expressions first
// lecture from 7.12
// that's done with 2 stacks

// https://dyingdown.github.io/2019/11/12/Calculator/
// The pictures from this look like the lecture

int Interpreter::evaluateTree(ExpressionTree::Node* tree)
{
    if(!tree)
    {
        return 0;
    }
    bool noRoots = (!tree->left && !tree->right);
    int value = 0;
    if(tree->data.keywrd == INT)
    {
        value = atoi(tree->data.data.c_str());
    }
    else if(tree->data.keywrd == VAR)
    {
        value = varHandler.getValue(tree->data.data);
    }
    else
    {
        if(noRoots)
        {
            throw std::runtime_error("Invalid expression");
            return 0;
        }
        return operation(evaluateTree(tree->left), evaluateTree(tree->right), tree->data.data);
    }
    if(noRoots)
    {
        return value;
    }
    if(tree->left)
    {
        evaluateTree(tree->left);
    }
    if(tree->right)
    {
        evaluateTree(tree->right);
    }
}

int Interpreter::evaluateExpression(const std::string& expr)
{
    ExpressionTree::Node* tree;
    tree = expTreeHelper.generate(expr);
    return evaluateTree(tree);

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
    return evaluateExpression(str);
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
                    //this will GOTO the first LABEL with the same
                    //string that it finds
                    if(tokens[j].data == tokens[i].data)
                    {
                        // the i++ from the for loop makes it the right index
                        i = --j;
                        continue;
                    }
                }
            }
        }
        else if(tokens[i].keywrd < numOfFuncs)
        {
            int value = (this->*functions[tokens[i].keywrd])(tokens[i].data);
            // Note: maybe i should add a condition for else if
            // Or maybe not
            if(tokens[i].keywrd == IF)
            {
                int endifIndex = getClosingToken(IF, ENDIF, tokens, i);
                if(endifIndex < 0)
                {
                    throw std::runtime_error("Couldn't find match ENDIF");
                }
                if(!value)
                {
                    int elseIndex = getClosingToken(IF, ELSE, tokens, i);
                    if(elseIndex > 0)
                    {
                        i = --elseIndex;
                    }
                    else
                    {
                        i = --endifIndex;
                    }
                    
                }
                
            }
        }
    }
}

//TODO MAKE THIS BETTER
//EITHER WITH MAP OR UNORDERED_MAP
int Interpreter::operation(int x, int y, const std::string& op)
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

int Interpreter::getClosingToken(keyWord opening, keyWord closing,
                            std::vector<Token>& tokens, size_t& start)
{
    size_t sz = tokens.size();
    int count = 0;
    for(int i = start; i < sz; i++)
    {
        if(tokens[i].keywrd == opening)
        {
            count++;
        }
        else if(tokens[i].keywrd == closing)
        {
            count--;
            if(count == 0)
            {
                return i;
            }
        }
    }
    return -1;
}