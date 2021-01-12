#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <map>
#include <iostream>


enum keyWord {LET, READ, PRINT, WHILE, DONE, IF, ELSE, ENDIF, ASSIGN, GOTO, LABEL, INT, OPERATOR};

struct Token
{
    keyWord keywrd;
    std::string data;
};

class Parser
{
    public:
        std::vector<Token> parseFile(std::string& filename);
        std::vector<Token> parseString(const std::string& str);
        std::vector<Token> parseExpression(const std::string& expr);
    private:
        bool isNumber(const char& c);
        bool isBracket(const char& c);
        bool isBlank(const char& c);
        bool isNewline(const char& c);
        void removeBlanks(std::string& str);

};

#endif //__PARSER_HPP__