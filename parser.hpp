#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <map>
#include <iostream>


enum keyWord {INT, OPERATOR, LET, READ, PRINT, GOTO, LABEL, WHILE, DONE, IF, ENDIF, ASSIGN};

struct Token
{
    keyWord keywrd;
    std::string data;
};

class Parser
{
    public:
        void parseFile(std::string& filename);
        std::vector<Token> parseString(const std::string& str);
        std::vector<Token> parseExpression(std::string& expr);
    private:
        bool isNumber(const char& c);
        bool isBracket(const char& c);

};

#endif //__PARSER_HPP__