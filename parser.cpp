#include "parser.hpp"

std::vector<Token> Parser::parseFile(std::string &filename)
{
    std::ifstream file(filename);
    if (file)
    {
        std::stringstream ss;
        ss << file.rdbuf();
        file.close();
        return parseString(ss.str());
    }
    std::vector<Token> empty;
    return empty;
}
std::vector<Token> Parser::parseString(const std::string &str)
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
        {"ASSIGN", ASSIGN},
        {"GOTO", GOTO},
        {"LABEL", LABEL}
        // {"INT", INT},
        // {"OPERATOR", OPERATOR}
    };

    std::vector<Token> toReturn;

    //std::cout << str;
    size_t sz = str.size();
    std::string buff;
    std::string buffPrev;
    keyWord key;
    bool foundKeyword = false;
    bool pushBlank = false;
    bool pushedBlank = false;
    for (size_t i = 0; i < sz; i++)
    {
        bool isLast = i == sz - 1;
        buff.push_back(str[i]);
        if (isNewline(str[i]) || isLast || pushBlank)
        {
            if (foundKeyword)
            {
                if(pushBlank)
                {
                    toReturn.push_back({key, ""});
                    pushBlank = false;
                }
                else
                {
                    removeBlanks(buff);
                    toReturn.push_back({key, buff});
                    
                }
                buff.clear();
                foundKeyword = false;
            }        
        }
        if(isspace(str[i]))
        {
            if (!foundKeyword || pushedBlank)
            {
                pushedBlank = false;
                removeBlanks(buff);
                if (!buff.empty())
                {
                    std::map<std::string, int>::const_iterator it = keyWordMap.find(buff);
                    if (it != keyWordMap.end())
                    {
                        key = (keyWord)it->second;
                        foundKeyword = true;
                        pushedBlank = pushBlank = (key == ENDIF || key == DONE);
                        buff.clear();
                    }
                    else if (!buff.compare("="))
                    {
                        key = ASSIGN;
                        foundKeyword = true;
                        buff = buffPrev + " " + buff;
                    }
                    else
                    {
                        buffPrev = buff;
                        buff.clear();
                    }
                }
            }
        }
    }
    //taking care of ending condition keyword being last
    if(!buff.empty())
    {
        std::map<std::string, int>::const_iterator it = keyWordMap.find(buff);
        if (it != keyWordMap.end())
        {
            toReturn.push_back({(keyWord)it->second, ""});
        }
    }
    return toReturn;
}

bool Parser::isNumber(const char &c)
{
    return c >= '0' && c <= '9';
}

bool Parser::isBracket(const char &c)
{
    return c == '(' || c == ')';
}

bool Parser::isBlank(const char &c)
{
    return c == ' ' || c == '\t';
}

bool Parser::isNewline(const char &c)
{
    return c == '\r' || c == '\n';
}

bool Parser::isValidVar(const char& c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c == '_');
}

keyWord Parser::numOrVar(std::string& str)
{
    if(str.empty())
    {
        //EMPTY STRING IS 0
        return INT;
    }
    bool isNum = false;
    bool isVar = false;
    bool isBad = false;
    for(const char& c : str)
    {
        if(isNumber(c) || c == '-')
        {
            isNum = true;
        }
        else if(isValidVar(c))
        {
            isVar = true;
        }
        else
        {
            isBad = true;
        }
    }
    if(isBad || (isNum && isVar))
    {
        throw std::runtime_error(str + " is not a valid variable name");
    }
    return isNum ? INT : VAR;
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