#ifndef __INTERPRETER_HPP__
#define __INTERPRETER_HPP__

#include <stack>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <list>

#include "parser.hpp"
#include "varhandler.hpp"
#include "expressiontree.hpp"

class Interpreter
{
    public:
        Parser parser;
        ExpressionTree expTreeHelper;
        int evaluateTree(ExpressionTree::Node* tree);
        ExpressionTree::Node* optimizeTree(ExpressionTree::Node* tree);
        int evaluateExpression(const std::string& expr);
        void interpretTokens(std::vector<Token> tokens);
        void repl();
    private:
        VarHandler varHandler;
        typedef std::unordered_map<std::string, ExpressionTree::Node*> exprToPoint;
        exprToPoint optimizedExpressions;
        int getOpPriority(const std::string& op);
        int binop(int x, int y, const std::string& op);
        int unop(int x, const std::string& op);
        void performLastOp(std::stack<int>& operands, std::stack<std::string>& operators);
        int getClosingToken(keyWord opening, keyWord closing,
                            std::vector<Token>& tokens, size_t& start);
        //
        std::string getArrayExpr(const std::string& expr);
        std::string getArrayName(const std::string& expr);
        int getArrayIndex(const std::string& expr);
        //
        int _let(const std::string& str);
        int _read(const std::string& str);
        int _print(const std::string& str);
        int _eval(const std::string& str);
        int _blank(const std::string& str);
};

#endif //__INTERPRETER_HPP__