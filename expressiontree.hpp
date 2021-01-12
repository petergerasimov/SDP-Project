#ifndef __EXPRESSION_TREE_HPP__
#define __EXPRESSION_TREE_HPP__

#include "helper.hpp"

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
        void optimize(Node* exprTree);
};

#endif //__EXPRESSION_TREE_HPP__