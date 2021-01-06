#ifndef __PARSER_HPP__
#define __PARSER_HPP__


#include <string>
#include <list>

struct TreeNode
{
    int data;
    std::list<TreeNode*> children;
};

struct Token
{
    int a;
};
// TODO
// Make a parser for expressions first
// lecture from 7.12
// that's done with 2 stacks
class Parser
{
    public:
        void parseFile(std::string filename);
        void parseString(std::string str);
    private:
};

#endif //__PARSER_HPP__