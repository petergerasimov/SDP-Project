#include "expressiontree.hpp"

ExpressionTree::Node* ExpressionTree::generate(std::string expr)
{
    std::stack<Node*> operands;
    std::stack<std::string> operators;

    std::vector<Token> tokens = parser.parseExpression(expr);

    //Variables to keep track of unary operators
    bool isPrevOp = true;
    Node* unaryOps = nullptr;

    int sz = tokens.size();
    for(int i = 0; i < sz; i++)
    {
        //Add variables and integers to the operands stack
        if(tokens[i].keywrd == VAR ||
           tokens[i].keywrd == INT)
        {
            if(unaryOps)
            {
                insertAtBottomRight(unaryOps, tokens[i]);
                operands.push(unaryOps);
                unaryOps = nullptr;
            }
            else
            {
                operands.push(new Node(tokens[i], nullptr, nullptr));
            }
            isPrevOp = false;
        }
        else if(tokens[i].keywrd == OPERATOR)
        {
            //Add opening bracket
            if(!tokens[i].data.compare("("))
            {
                operators.push(tokens[i].data);
            }
            //Constructing tree until an opening bracket
            else if(!tokens[i].data.compare(")"))
            {
                while (!operators.empty() && 
                        operators.top().compare("("))
                {
                    constructBinOpNode(operands, operators);
                }
                if(!operators.empty())
                {
                    operators.pop();
                }
                
                //Brackets are operators but they act different...
                isPrevOp = false;
            }

            else if(isPrevOp && tokens[i].data.compare("("))
            {
                if(unaryOps)
                {
                    insertAtBottomRight(unaryOps, tokens[i]);
                }
                else
                {
                    unaryOps = new Node(tokens[i], nullptr, nullptr);
                }
            }
            
            else
            {
                if(!operators.empty())
                {
                    int opTop = getOpPriority(operators.top());
                    int opCurr = getOpPriority(tokens[i].data);
                    //Check if top operator is of higher priority
                    //The assume left associativity
                    //Unless operator =
                    if(opTop > opCurr || ((opTop == opCurr && tokens[i].data.compare("="))))
                    {
                        constructBinOpNode(operands, operators);
                    }
                }

                //If not of higher priority add to stack
                operators.push(tokens[i].data);
                isPrevOp = true;
            }
        }
        
    }
    //Construct tree from remaining elements
    while(!operators.empty())
    {
        if(!operators.top().compare(")"))
        {
            operators.pop();
        }
        else
        {
            constructBinOpNode(operands, operators);
        }
    }
    if(!operands.empty())
    {
        return operands.top();
    }
    return nullptr;
}
void ExpressionTree::constructBinOpNode(std::stack<Node*>& operands, 
                                        std::stack<std::string>& operators)
{
    if(operands.size() < 2)
    {
        // std::cout << "Can't find 2 operands" << std::endl;
        return;
    }
    Node* rhs = operands.top();
    operands.pop();
    Node* lhs = operands.top();
    operands.pop();
    Node* stitched = new Node({OPERATOR, operators.top()}, lhs, rhs);
    operands.push(stitched);
    operators.pop();
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

void ExpressionTree::insertAtBottomRight(Node* tree, Token& toInsert)
{
    if(!tree)
    {
        return;
    }
    else
    {
        if(!tree->right)
        {
            tree->right = new Node(toInsert, nullptr, nullptr);
        }
        else
        {
            insertAtBottomRight(tree->right, toInsert);
        }
    }
}