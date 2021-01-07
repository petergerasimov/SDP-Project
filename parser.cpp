#include "parser.hpp"

#include <fstream>
#include <cstring>
#include <iostream>

void Parser::parseFile(std::string filename)
{
    std::ifstream file(filename);
    if(!file)
    {
        return;
    }
    bool isPrevKeyword = false;
    while(!file.eof())
    {
        std::string tmp;
        file >> tmp;
        // if(tmp == )
        // {

        // }
    }
}
void Parser::parseString(std::string str)
{

}

bool Parser::isNumber(char c)
{
    return c >= '0' && c <= '9';
}

bool Parser::isBracket(char c)
{
    return c == '(' || c == ')';
}

std::vector<Token> Parser::parseExpression(std::string expr)
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
                toReturn.push_back({OPERATOR, curr});
                curr.clear();
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
                if(isBracket(c))
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
        if(isNumber(curr[0]))
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