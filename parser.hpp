#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <string>
#include <list>

struct TreeNode
{
    int data;
    std::list<TreeNode*> children;
};

enum keyWord {LET, READ, PRINT, GOTO, LABEL, WHITE, IF, ENDIF};

struct Token
{
    keyWord keywrd;
    std::string data;
};

class Parser
{
    public:
        void parseFile(std::string filename);
        void parseString(std::string str);
    private:
};

#endif //__PARSER_HPP__