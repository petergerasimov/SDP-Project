#include "expressiontree.hpp"

ExpressionTree::Node* ExpressionTree::generate(std::string expr)
{
    std::stack<Node*> operands;
    std::stack<std::string> operators;

    std::vector<Token> tokens = parser.parseExpression(expr);

    bool isPrevOp = false;

    int sz = tokens.size();
    for(int i = sz - 1; i >= 0; i--)
    {
        // std::cout << tokens[i].data << " " << (isPrevOp ? "YES": "NO") << std::endl;
        // If token is an int or var, add it
        if(tokens[i].keywrd == INT || tokens[i].keywrd == VAR)
        {
            Node* toAdd = new Node(tokens[i], nullptr, nullptr);
            operands.push(toAdd);
            isPrevOp = false;
        }
        else if (tokens[i].keywrd == OPERATOR)
        {
            std::string prevOp;
            // Get the prev operator
            if(!operators.empty() && operators.top().compare(")"))
            {
                prevOp = operators.top();
            }
            // Skip opening bracket when adding
            if(tokens[i].data.compare("("))
            {
                operators.push(tokens[i].data);
            }
            if(!operators.empty())
            {
                // If token is opening bracket
                // Do all operations until closing bracket
                if(!tokens[i].data.compare("("))
                {
                    isPrevOp = false;
                    //TODO: check if there are the right amount of brackets
                    while(operators.top().compare(")"))
                    {
                        constructBinOpNode(operands, operators.top());
                        operators.pop();
                    }
                    operators.pop();
                }
                else if(operators.top().compare(")"))
                {
                    if(isPrevOp)
                    {
                        Node* topOperand = operands.top();
                        operands.pop();
                        operands.push(new Node({OPERATOR, prevOp}, nullptr, topOperand));
                        // Pop prev op
                        std::string tmp = operators.top();
                        operators.pop();
                        operators.pop();
                        operators.push(tmp);
                    }
                    else if(operands.size() >= 2 && !prevOp.empty())
                    {
                        // If prev op is of higher priority build tree up
                        // Not sure if this should be > or >=
                        // TODO: priorities are messed up in cases like 5+5*5/7
                        if(getOpPriority(prevOp) > getOpPriority(operators.top()))
                        {
                            constructBinOpNode(operands, prevOp);
                            // Pop prev op
                            std::string tmp = operators.top();
                            operators.pop();
                            operators.pop();
                            operators.push(tmp);
                        }
                    }
                    isPrevOp = true;
                }
                else
                {
                    isPrevOp = false;
                }
                
            }
            
        }
    }
    // If there are any operators left
    // Build the tree up further
    while(!operators.empty())
    {
        if(!operators.top().compare(")"))
        {
            operators.pop();
            continue;
        }
        constructBinOpNode(operands, operators.top());
        operators.pop();
    }
    if(!operands.empty())
    {
        return operands.top();
    }
    return nullptr;
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