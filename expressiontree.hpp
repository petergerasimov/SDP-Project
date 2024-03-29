#ifndef __EXPRESSION_TREE_HPP__
#define __EXPRESSION_TREE_HPP__

#include <stack>
#include "helper.hpp"
#include "parser.hpp"

class ExpressionTree
{
    public:
        struct Node
        {
            Token data;
            Node* left;
            Node* right;
            Node(Token data, Node* left, Node* right) :
                data(data), left(left), right(right) {}
        };
        Node* generate(std::string expr);
        Node* copyTree(Node* tree);
        void deleteTree(Node* tree);
    private:
        Parser parser;
        int getOpPriority(const std::string& op);
        void constructBinOpNode(std::stack<Node*>& operands, 
                                std::stack<std::string>& operators);
        void insertAtBottomRight(Node* tree, Token& toInsert);
};

#endif //__EXPRESSION_TREE_HPP__