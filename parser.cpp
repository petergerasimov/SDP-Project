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
    // enum keyWord {INT, OPERATOR, LET, READ, PRINT, GOTO, LABEL, WHILE, DONE, IF, ELSE, ENDIF, ASSIGN};
    static const std::map<std::string, int> keyWordMap = {
        {"INT", INT},
        {"OPERATOR", OPERATOR},
        {"LET", LET},
        {"READ", READ},
        {"PRINT", PRINT},
        {"GOTO", GOTO},
        {"LABEL", LABEL},
        {"WHILE", WHILE},
        {"DONE", DONE},
        {"IF", IF},
        {"ELSE", ELSE},
        {"ENDIF", ENDIF},
        {"ASSIGN", ASSIGN}};

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
        if (isNewline(str[i]) || isLast)
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
                    buff.clear();
                }
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

std::vector<Token> Parser::parseExpression(std::string &expr)
{
    std::vector<Token> toReturn;

    bool isPrevNumber = true;
    bool isPrevClosingBracket = false;
    std::string curr;
    for (const char &c : expr)
    {
        if (c == ' ')
        {
            continue;
        }
        else if (c == '-')
        {
            if (!isPrevNumber || expr.empty())
            {
                if (isPrevClosingBracket)
                {
                    toReturn.push_back({OPERATOR, curr});
                    toReturn.push_back({OPERATOR, {c}});
                    curr.clear();
                    isPrevNumber = false;
                }
                else
                {
                    toReturn.push_back({OPERATOR, curr});
                    curr.clear();
                    curr.push_back(c);
                    isPrevNumber = true;
                }
            }
            else
            {
                toReturn.push_back({INT, curr});
                curr.clear();
                curr.push_back(c);
                isPrevNumber = false;
            }
            isPrevClosingBracket = false;
        }
        else if (isNumber(c))
        {
            if (isPrevNumber)
            {
                curr.push_back(c);
            }
            else
            {
                if (!curr.empty())
                {
                    toReturn.push_back({OPERATOR, curr});
                    curr.clear();
                }
                curr.push_back(c);
            }
            isPrevNumber = true;
            isPrevClosingBracket = false;
        }
        else
        {
            if (!isPrevNumber)
            {
                if (isBracket(curr[0]))
                {
                    toReturn.push_back({OPERATOR, curr});
                    curr.clear();
                }
                if (isBracket(c) && !curr.empty())
                {
                    toReturn.push_back({OPERATOR, curr});
                    curr.clear();
                }
                curr.push_back(c);
            }
            else
            {
                if (!curr.empty())
                {
                    toReturn.push_back({INT, curr});
                    curr.clear();
                }
                curr.push_back(c);
            }
            isPrevClosingBracket = (c == ')');
            isPrevNumber = false;
        }
    }
    if (!curr.empty())
    {
        if (isNumber(curr[0]) || curr[0] == '-')
        {
            toReturn.push_back({INT, curr});
        }
        else
        {
            toReturn.push_back({OPERATOR, curr});
        }
    }

    return toReturn;
}