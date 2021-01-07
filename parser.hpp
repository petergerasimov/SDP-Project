#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <vector>
#include <string>
#include <list>

struct TreeNode
{
    int data;
    std::list<TreeNode*> children;
};

// enum keyWord {LET, READ, PRINT, GOTO, LABEL, WHITE, IF, ENDIF};
enum keyWord {INT, OPERATOR};

struct Token
{
    keyWord keywrd;
    std::string data;
};

class Parser
{
    public:
        void parseFile(std::string& filename);
        void parseString(std::string& str);
        std::vector<Token> parseExpression(std::string& expr);
    private:
        bool isNumber(const char& c);
        bool isBracket(const char& c);

};

#endif //__PARSER_HPP__