#include "parser.hpp"

void Parser::parseFile(std::string& filename)
{
    std::ifstream file(filename);
    if(file)
    {
        std::stringstream ss;
        ss << file.rdbuf();
        parseString(ss.str());
        file.close();
    }
}
std::vector<Token> Parser::parseString(const std::string& str)
{
    // enum keyWord {INT, OPERATOR, LET, READ, PRINT, GOTO, LABEL, WHILE, DONE, IF, ENDIF, ASSIGN};
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
        {"ENDIF", ENDIF},
        {"ASSIGN", ASSIGN}
    };

    std::vector<Token> toReturn;

    //std::cout << str;
    size_t sz = str.size();
    std::string buff;
    for(size_t i = 0; i < sz; i++)
    {
        if(!isspace(str[i]))
        {
            buff.push_back(str[i]);
        }
        else if(!buff.empty())
        {
            std::map<std::string, int>::const_iterator it = keyWordMap.find(buff);
            if(it != keyWordMap.end())
            {
                std::cout << it->first << " " << it->second << std::endl;
            }
            buff.clear();
        }
        
    }
    return toReturn;
}

bool Parser::isNumber(const char& c)
{
    return c >= '0' && c <= '9';
}

bool Parser::isBracket(const char& c)
{
    return c == '(' || c == ')';
}

std::vector<Token> Parser::parseExpression(std::string& expr)
{
    std::vector<Token> toReturn;

    bool isPrevNumber = true;
    bool isPrevClosingBracket = false;
    std::string curr;
    for(const char& c : expr)
    {   
        if(c == ' ')
        {
            continue;
        }
        else if (c == '-')
        {
            if(!isPrevNumber || expr.empty())
            {
                if(isPrevClosingBracket)
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
        else if(isNumber(c))
        {
            if(isPrevNumber)
            {
                curr.push_back(c);
            }
            else
            {
                if(!curr.empty())
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
            if(!isPrevNumber)
            {
                if(isBracket(curr[0]))
                {
                    toReturn.push_back({OPERATOR, curr});
                    curr.clear();
                }
                if(isBracket(c) && !curr.empty())
                {
                    toReturn.push_back({OPERATOR, curr});
                    curr.clear();
                }
                curr.push_back(c);
            }
            else
            {
                if(!curr.empty())
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
    if(!curr.empty())
    {
        if(isNumber(curr[0]) || curr[0] == '-')
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