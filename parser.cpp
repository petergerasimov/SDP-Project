#include "parser.hpp"

std::vector<Token> Parser::parseFile(std::string &filename)
{
    std::ifstream file(filename);
    if (file)
    {
        std::stringstream ss;
        ss << file.rdbuf();
        file.close();
        return parseStream(ss);
    }
    std::vector<Token> empty;
    return empty;
}

std::vector<Token> Parser::parseStream(std::stringstream& ss)
{
    //enum keyWord {LET, READ, PRINT, WHILE, DONE, IF, ELSE, ENDIF, ASSIGN, GOTO, LABEL, INT, VAR, OPERATOR};
    static const std::map<std::string, int> keyWordMap = {
        {"LET", LET},
        {"READ", READ},
        {"PRINT", PRINT},
        {"WHILE", WHILE},
        {"DONE", DONE},
        {"IF", IF},
        {"ELSE", ELSE},
        {"ENDIF", ENDIF},
        {"GOTO", GOTO},
        {"LABEL", LABEL}
    };

    std::vector<Token> toReturn;

    while(!ss.eof())
    {
        std::string buff;
        ss >> buff;
        std::map<std::string, int>::const_iterator it;
        it = keyWordMap.find(buff);
        if(it != keyWordMap.end())
        {
            if(it->second != DONE && it->second != ENDIF)
            {
                std::string expr;
                std::getline(ss, expr);
                removeBlanks(expr);
                toReturn.push_back({(keyWord)it->second, expr});
            }
            else
            {
                toReturn.push_back({(keyWord)it->second, ""});
            }
        }
        else
        {
            std::string expr;
            std::getline(ss, expr);
            expr = buff + expr;
            removeBlanks(expr);
            toReturn.push_back({ASSIGN, expr});
        }
    }
    return toReturn;
}
std::vector<Token> Parser::parseString(const std::string &str)
{
    std::stringstream ss;
    ss << str;
    return parseStream(ss);
}

bool Parser::isNumber(const char &c)
{
    return c >= '0' && c <= '9';
}

bool Parser::isValidVar(const char& c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c == '_');
}

keyWord Parser::numOrVar(const std::string& str)
{
    size_t sz = str.size();
    if(str.empty())
    {
        throw std::runtime_error("Empty string is not a variable");
    }
    if(str[0] == '-' || isNumber(str[0]))
    {
        for(size_t i = 1; i < sz; i++)
        {
            if(!isNumber(str[i]))
            {
                throw std::runtime_error(str + " is not a valid variable.");
            }
        }
        return INT;
    }
    else if(isValidVar(str[0]))
    {
        for(size_t i = 1; i < sz; i++)
        {
            if(str[i] == '[')
            {
                if(str[sz - 1] == ']')
                {
                    numOrVar(str.substr(i + 1, sz - i - 2));
                }
                else
                {
                    throw std::runtime_error(str + " is not a valid variable.");
                }
            }
            else if(!isValidVar(str[i]) && !isNumber(str[i]) && !(str[i] == ']'))
            {
                throw std::runtime_error(str + " is not a valid variable.");
            }
        }
        // TODO: Maybe it will be more optimal if there is an ARRAY keyword
        return VAR;
    }
    else
    {
        throw std::runtime_error(str + " is not a valid variable.");
    }
    return VAR;
}

void Parser::removeBlanks(std::string &str)
{
    std::string tmp;
    for (const char &c : str)
    {
        if (!isspace(c))
        {
            tmp.push_back(c);
        }
    }
    str = tmp;
}

std::vector<Token> Parser::parseExpression(const std::string &expr)
{
    // И оператори =, ==, !=, <, <=, >, >=, +, *, /, -, %, &&, ||, !,()
    enum Operators {ASGN, EQ, NOTEQ, LESS, LESSEQ,
                    GRTR, GRTREQ, PLUS, MUL, DIV,
                    MINUS, MOD, AND, OR, NOT, OPBR, CLOSEBR};

    static const std::map<std::string, int> opMap = {
        {"=" , ASGN},
        {"==", EQ},
        {"!=", NOTEQ},
        {"<" , LESS},
        {"<=", LESSEQ},
        {">" , GRTR},
        {">=", GRTREQ},
        {"+" , PLUS},
        {"*" , MUL},
        {"/" , DIV},
        {"-" , MINUS},
        {"%" , MOD},
        {"&&", AND},
        {"||", OR},
        {"!" , NOT},
        {"(" , OPBR},
        {")" , CLOSEBR}
    };
    std::vector<Token> toReturn;

    std::string buffer;
    size_t sz = expr.size();
    for(size_t i = 0; i < sz; i++)
    {
        std::string toComp;
        std::string opToAdd;
        if(i + 1 < sz)
        {
            toComp = expr.substr(i, 2);
        }
        else
        {
            toComp = expr[i];
        }
        std::map<std::string, int>::const_iterator it;
        it = opMap.find(toComp);
        if(it != opMap.end())
        {
            opToAdd = toComp;
        }
        else
        {
            it = opMap.find({toComp[0]});
            if(it != opMap.end())
            {
                opToAdd = toComp[0];
            }
        }
        // IF AN OPERATOR IS FOUND
        if(!opToAdd.empty())
        {
            if(!buffer.empty())
            {
                keyWord key = numOrVar(buffer);
                toReturn.push_back({key, buffer});
                buffer.clear();
            }
            if(opToAdd.size() == 2)
            {
                ++i;
            }
            toReturn.push_back({OPERATOR, opToAdd});
        }
        else if(!isspace(expr[i]))
        {
            buffer.push_back(expr[i]);
        }
    }
    //add leftovers
    if(!buffer.empty())
    {
        keyWord key = numOrVar(buffer);
        toReturn.push_back({key, buffer});
        buffer.clear();
    }
    return toReturn;
}