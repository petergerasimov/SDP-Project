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
                toReturn.push_back({numOrVar(curr), curr});
                curr.clear();
                curr.push_back(c);
                isPrevNumber = false;
            }
            isPrevClosingBracket = false;
        }
        // Note to self: Added var to here
        else if (isNumber(c) || isValidVar(c))
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
                    toReturn.push_back({numOrVar(curr), curr});
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
            toReturn.push_back({numOrVar(curr), curr});
        }
        else
        {
            toReturn.push_back({OPERATOR, curr});
        }
    }

    return toReturn;
}

// std::vector<Token> Parser::parseExpression(const std::string &expr)
// {
//     std::vector<Token> toReturn;
//     std::string buffer;
//     bool isPrevNumber = false;
//     bool isPrevVar = false;
//     bool isPrevOp = true;
//     bool isPrevClosingBracket = false;
//     for(const char& c : expr)
//     {
//         if(isspace(c))
//         {
//             continue;
//         }
//         else if(isNumber(c))
//         {
//             if(!isPrevNumber)
//             {
//                 keyWord key = isPrevVar ? VAR : OPERATOR;
//                 if(!buffer.empty())
//                     toReturn.push_back({key, buffer});
//                 buffer.clear();
//             }
//             buffer.push_back(c);
//             isPrevNumber = true;
//             isPrevVar = false;
//             isPrevOp = false;
//             isPrevClosingBracket = false;
//         }
//         else if(isValidVar(c))
//         {
//             if(!isPrevVar)
//             {
//                 keyWord key = isPrevNumber ? INT : OPERATOR;
//                 if(!buffer.empty())
//                     toReturn.push_back({key, buffer});
//                 buffer.clear();
//             }
//             buffer.push_back(c);
//             isPrevNumber = false;
//             isPrevVar = true;
//             isPrevOp = false;
//             isPrevClosingBracket = false;
//         }
//         // In this case I'm assuming it's an operator
//         else
//         {
//             if(!isPrevOp)
//             {
//                 keyWord key = isPrevNumber ? INT : VAR;
//                 if(!buffer.empty())
//                     toReturn.push_back({key, buffer});
//                 buffer.clear();
//                 buffer.push_back(c);
//             }
//             else if(isPrevClosingBracket)
//             {
//                 if(!buffer.empty())
//                     toReturn.push_back({OPERATOR, buffer});
//                 // toReturn.push_back({OPERATOR, {c}});
//                 buffer.clear();
//                 buffer.push_back(c);
//             }
//             //Last was operator and not closing bracket
//             else
//             {
//                 if(c == '-' || c == '+')
//                 {
//                     toReturn.push_back({INT, {'0'}});
//                     toReturn.push_back({OPERATOR, {c}});
//                 }
//                 else if(isBracket(c))
//                 {
//                     toReturn.push_back({OPERATOR, {c}});
//                 }
//                 else
//                 {
//                     buffer.push_back(c);
//                 }
                
//             }
//             isPrevNumber = false;
//             isPrevVar = false;
//             isPrevOp = true;
//             isPrevClosingBracket = (c == ')');
//         }
        
//     }
    
//     return toReturn;
// }