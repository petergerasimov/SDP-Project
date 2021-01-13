#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <map>
#include <iostream>

#include "helper.hpp"

class Parser
{
    public:
        std::vector<Token> parseFile(std::string& filename);
        std::vector<Token> parseString(const std::string& str);
        std::vector<Token> parseStream(std::stringstream& ss);
        std::vector<Token> parseExpression(const std::string& expr);
    private:
        bool isNumber(const char& c);
        bool isBracket(const char& c);
        bool isBlank(const char& c);
        bool isNewline(const char& c);
        bool isValidVar(const char& c);

        keyWord numOrVar(std::string& str);

        void removeBlanks(std::string& str);

};

#endif //__PARSER_HPP__