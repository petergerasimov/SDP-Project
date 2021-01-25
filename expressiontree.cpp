#include "expressiontree.hpp"

ExpressionTree::Node* ExpressionTree::generate(std::string expr)
{
    std::stack<Node*> operands;
    std::stack<std::string> operators;

    std::vector<Token> tokens = parser.parseExpression(expr);

    // TODO: i think this should be read from end to begin
    // To fix priorities
    int sz = tokens.size();
    for(int i = sz - 1; i >= 0; i--)
    {
        if(tokens[i].keywrd == INT || tokens[i].keywrd == VAR)
        {
            Node* toAdd = new Node(tokens[i], nullptr, nullptr);
            operands.push(toAdd);
        }
        else if (tokens[i].keywrd == OPERATOR)
        {
            std::string prevOp;
            if(!operators.empty())
            {
                prevOp = operators.top();
            }
            if(tokens[i].data.compare("("))
            {
                operators.push(tokens[i].data);
            }
            if(!operators.empty())
            {
                if(!tokens[i].data.compare("("))
                {
                    //TODO: check if there are the right amount of brackets
                    while(operators.top().compare(")"))
                    {
                        // std::cout << operators.top() << "<- top prev ->" << prevOp << std::endl;
                        // std::cout << "operand size " << operands.size() << std::endl;
                        constructBinOpNode(operands, operators.top());
                        operators.pop();
                    }
                    // operators.pop();
                }
                // This thing works for now :DD 
                if(operands.size() >= 2 && !prevOp.empty() && getOpPriority(prevOp) > getOpPriority(operators.top()))
                {
                    if(operators.top().compare(")"))
                    {
                        constructBinOpNode(operands, prevOp);
                        std::string tmp = operators.top();
                        operators.pop();
                        operators.pop();
                        operators.push(tmp);
                    }
                }
            }
            
        }
    }
    while(!operators.empty())
    {
        constructBinOpNode(operands, operators.top());
        operators.pop();
    }
    
    return operands.top();
}
void ExpressionTree::constructBinOpNode(std::stack<Node*>& operands, 
                                        std::string& op)
{
    if(operands.size() < 2)
    {
        // throw std::runtime_error("Can't find 2 operands");
        std::cout << "Can't find 2 operands" << std::endl;
        return;
    }
    Node* lhs = operands.top();
    operands.pop();
    Node* rhs = operands.top();
    operands.pop();
    // std::cout << op << ": " << lhs->data.data << " and " << rhs->data.data << std::endl;
    Node* stitched = new Node({OPERATOR, op}, lhs, rhs);
    operands.push(stitched);
}
void ExpressionTree::optimize(Node* exprTree)
{

}
int ExpressionTree::getOpPriority(const std::string& op)
{
    static const std::map<std::string, int> opPriority = {
        {"(" , 0},
        {"=" , 1},
        {"||", 2},
        {"&&", 3},
        {"==", 4},
        {"!=", 4},
        {"<" , 5},
        {"<=", 5},
        {">" , 5},
        {">=", 5},
        {"+" , 6},
        {"-" , 6},
        {"*" , 7},
        {"/" , 7},
        {"%" , 7},
        {"!" , 8},
        {")" , 9}
    };

    std::map<std::string, int>::const_iterator it = opPriority.find(op);
    if (it != opPriority.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("Invalid operator " + op);
        return -1;
    }
}