#include "interpreter.hpp"

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
        value = varHandler.getValue(getArrayName(tree->data.data), 
                                    getArrayIndex(tree->data.data));
    }
    else
    {
        if(noRoots)
        {
            throw std::runtime_error("Invalid expression");
            return 0;
        }
        if(!tree->left && tree->right)
        {
            return unop(evaluateTree(tree->right), tree->data.data);
        }
        if(!tree->data.data.compare("="))
        {
            int rvalue = -1;
            if(tree->left->data.keywrd == VAR)
            {
                rvalue = evaluateTree(tree->right);
                varHandler.changeValue(getArrayName(tree->left->data.data), 
                                       rvalue,
                                       getArrayIndex(tree->left->data.data));
            }
            else
            {
                throw std::runtime_error(tree->left->data.data + " is not an lvalue.");
            }
            return rvalue;
        }
        return binop(evaluateTree(tree->left), evaluateTree(tree->right), tree->data.data);
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
    return 0;
}

ExpressionTree::Node* Interpreter::optimizeTree(ExpressionTree::Node* tree)
{
    if(!tree)
    {
        return nullptr;
    }
    ExpressionTree::Node* left = nullptr;
    ExpressionTree::Node* right = nullptr;
    if(tree->data.keywrd == OPERATOR)
    {
        if(tree->left)
        {
            if(tree->left->data.keywrd == OPERATOR)
            {
                left = optimizeTree(tree->left);
            }
            else
            {
                left = tree->left;
            }
        }
        if(tree->right)
        {
            if(tree->right->data.keywrd == OPERATOR)
            {
                right = optimizeTree(tree->right);
            }
            else
            {
                right = tree->right;
            }
            if(right->data.keywrd == INT)
            {
                if(!tree->left)
                {
                    tree->data.data = std::to_string(unop(atoi(right->data.data.c_str()), tree->data.data));
                    tree->data.keywrd = INT;
                    delete tree->right;
                    tree->right = nullptr;
                }
                else
                {
                    if(left->data.keywrd == INT)
                    {
                        tree->data.data = std::to_string(binop(atoi(left->data.data.c_str()),
                                                               atoi(right->data.data.c_str()), 
                                                               tree->data.data));
                        tree->data.keywrd = INT;
                        delete tree->left;
                        tree->left = nullptr;
                        delete tree->right;
                        tree->right = nullptr;
                    }
                    
                }
            }
            
        }
    }
    return tree;
}

int Interpreter::evaluateExpression(const std::string& expr)
{
    exprToPoint::iterator it = optimizedExpressions.find(expr);
    if(it != optimizedExpressions.end())
    {
        return evaluateTree(it->second);
    }
    else
    {
        ExpressionTree::Node* tree;
        tree = expTreeHelper.generate(expr);
        optimizeTree(tree);
        optimizedExpressions.insert({expr, tree});
        return evaluateTree(tree);
    }
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
    operands.push(binop(y, x, operators.top()));
    operators.pop();
}

std::string Interpreter::getArrayExpr(const std::string& expr)
{
    std::string emptyExpr;
    size_t sz = expr.size();
    for(size_t i = 0; i < sz; i++)
    {
        if(expr[i] == '[')
        {
            if(expr[sz - 1] == ']')
            {
                return expr.substr(i + 1, sz - i - 2);
            }
        }
    }
    return emptyExpr;
}

std::string Interpreter::getArrayName(const std::string& expr)
{
    size_t sz = expr.size();
    for(size_t i = 0; i < sz; i++)
    {
        if(expr[i] == '[')
        {
            if(expr[sz - 1] == ']')
            {
                return expr.substr(0, i);
            }
        }
    }
    return expr;
}

int Interpreter::getArrayIndex(const std::string& expr)
{
    std::string arrExpr = getArrayExpr(expr);
    size_t index = 0;
    if(!arrExpr.empty())
    {
        index = evaluateExpression(arrExpr);
    }
    return index;
}


int Interpreter::_let(const std::string& str)
{
    varHandler.add(getArrayName(str), getArrayIndex(str));
    return 1;
}
int Interpreter::_read(const std::string& str)
{
    int tmp;
    std::cin >> tmp;
    varHandler.changeValue(getArrayName(str), tmp, getArrayIndex(str));
    return 1;
}
int Interpreter::_print(const std::string& str)
{
    std::cout << evaluateExpression(str);
    std::cout << std::endl;
    return 1;
}
int Interpreter::_eval(const std::string& str)
{
    return evaluateExpression(str);
}
int Interpreter::_blank(const std::string& str)
{
    std::cout << str;
    return 1;
}

void Interpreter::interpretTokens(std::vector<Token> tokens)
{
    std::stack<std::list<std::string>> validVars;
    std::list<std::string> empty;
    validVars.push(empty);
    
    typedef int (Interpreter::*func)(const std::string& str);
    // enum keyWord {LET, READ, PRINT, WHILE, DONE, IF, ELSE, ENDIF, ASSIGN, GOTO, LABEL, INT, OPERATOR};
    static const size_t numOfFuncs = 9;
    static const func functions[numOfFuncs] = {
        &Interpreter::_let,    //let
        &Interpreter::_read,   //read
        &Interpreter::_print,  //print
        &Interpreter::_eval,   //while
        &Interpreter::_blank,  //done
        &Interpreter::_eval,   //if
        &Interpreter::_blank,  //else
        &Interpreter::_blank,  //endif
        &Interpreter::_eval    //assign
    };
    
    int sz = tokens.size();
    std::stack<int> whileIndicies;
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
                //If is a new block
                validVars.push(empty);

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
            if(tokens[i].keywrd == ENDIF)
            {
                //Invalidate vars from current if
                if(!validVars.empty())
                {
                    for(const std::string& var : validVars.top())
                    {
                        varHandler.invalidate(var);
                    }
                    validVars.pop();
                }
            }
            else if(tokens[i].keywrd == WHILE)
            {
                // //While is a new block
                // validVars.push(empty);
                int doneIndex = getClosingToken(WHILE, DONE, tokens, i);
                if(doneIndex < 0)
                {
                    throw std::runtime_error("Couldn't find match DONE");
                }
                if(!value)
                {
                    i = doneIndex;
                }
                else
                {
                    //While is a new block
                    validVars.push(empty);
                    whileIndicies.push(i);
                }
            }
            else if(tokens[i].keywrd == DONE)
            {
                if(!whileIndicies.empty())
                {
                    int gotoWhile = whileIndicies.top();
                    whileIndicies.pop();
                    i = --gotoWhile;
                }
                //Invalidate vars from current while
                if(!validVars.empty())
                {
                    for(const std::string& var : validVars.top())
                    {
                        varHandler.invalidate(var);
                    }
                    validVars.pop();
                }
            }
            else if(tokens[i].keywrd == LET)
            {
                validVars.top().push_back(tokens[i].data);
            }
        }
    }
}

int Interpreter::binop(int x, int y, const std::string& op)
{
    typedef int (*opFunc)(int& x, int& y);
    typedef std::map<std::string, opFunc> strToOpMap;
    // Mapping operator string to a function
    static const strToOpMap opMap = {
        {"||", [](int& x, int& y)->int{return x || y;}},
        {"=" , [](int& x, int& y)->int{
            throw std::runtime_error(std::to_string(x) + " is not an lvalue.");
            return -1;
        }},
        {"&&", [](int& x, int& y)->int{return x && y;}},
        {"==", [](int& x, int& y)->int{return x == y;}},
        {"!=", [](int& x, int& y)->int{return x != y;}},
        {"<" , [](int& x, int& y)->int{return x <  y;}},
        {"<=", [](int& x, int& y)->int{return x <= y;}},
        {">" , [](int& x, int& y)->int{return x >  y;}},
        {">=", [](int& x, int& y)->int{return x >= y;}},
        {"+" , [](int& x, int& y)->int{return x +  y;}},
        {"-" , [](int& x, int& y)->int{return x -  y;}},
        {"*" , [](int& x, int& y)->int{return x *  y;}},
        {"/" , [](int& x, int& y)->int{
            if(y == 0) throw std::runtime_error("Can't divide by 0.");
            return x / y;
        }},
        {"%" , [](int& x, int& y)->int{
            if(y == 0) throw std::runtime_error("Can't divide by 0.");
            return x % y;
        }},
    };
    strToOpMap::const_iterator it = opMap.find(op);
    if(it != opMap.end())
    {
        return (*it->second)(x,y);
    }
    else
    {
        throw std::runtime_error("Can't find operator " + op);
        return -1;
    }
}

int Interpreter::unop(int x, const std::string& op)
{
    if(!op.compare("+"))
    {
        return x;
    }
    else if(!op.compare("-"))
    {
        return -x;
    }
    else if(!op.compare("!"))
    {
        return !x;
    }
    else
    {
        throw std::runtime_error(op + " not a unary operator");
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

void Interpreter::repl()
{
    std::cout << "Started in REPL mode\n"
              << "Type \"run\" when you finish coding\n"
              << "Type \"exit\" to exit\n";
    
    std::string toParse;
    std::vector<Token> tokens;
    while(true)
    {
        std::getline(std::cin, toParse);
        if(toParse == "run")
        {
            interpretTokens(tokens);
            // Not sure if prev tokens should clear after runing
            // tokens.clear(); 
        }
        else if(toParse == "exit")
        {
            break;
        }
        else
        {
            std::vector<Token> toAppend;
            toAppend = parser.parseString(toParse);
            tokens.insert(tokens.end(), toAppend.begin(), toAppend.end());
        }
    }
}