#include <iostream>
#include <cassert>
#include "../interpreter.hpp"

//Generate SVG from tree
void genStream(ExpressionTree::Node* tree, std::stringstream& ss, int prevPos = -1);
void toSVG(ExpressionTree::Node* tree, std::string filename);

int main()
{
    Interpreter i;
    int res;
    //Testing expressions
    res = i.evaluateExpression("1 + 1");
    assert(res == (1 + 1));
    res = i.evaluateExpression("1 + 1 + 5*5 + 3");
    assert(res == (1 + 1 + 5*5 + 3));
    res = i.evaluateExpression("(256 - 254)*2");
    assert(res == ((256 - 254)*2));
    res = i.evaluateExpression("(1354 - 1311)*(137 + 954) + 1337");
    assert(res == ((1354 - 1311)*(137 + 954) + 1337));
    res = i.evaluateExpression("5 == (7 - 2)");
    assert(res == (5 == (7 - 2)));
    res = i.evaluateExpression("5-+-+-+-+-+5)");
    assert(res == (5-+-+-+-+-+5));
    res = i.evaluateExpression("5 + 5*5/7");
    assert(res == (5 + 5*5/7));
    res = i.evaluateExpression("(7-2) + (5 - (3-5))");
    assert(res == ((7-2) + (5 - (3-5))));
    res = i.evaluateExpression("-1");
    assert(res == (-1));
    //Testing programs
    std::string filename = "variableTest.pep";
    std::vector<Token> tokens = i.parser.parseFile(filename);
    i.interpretTokens(tokens);
    std::cout << "No errors" << std::endl;
    return EXIT_SUCCESS;
}

void genStream(ExpressionTree::Node* tree, std::stringstream& ss, int prevPos = -1)
{
    static int nodePos = 0;
    nodePos++;
    if(!tree) return;
    if(prevPos != -1)
    {
        ss << "\tn" << prevPos << " -- n" << nodePos << ";\n";
    }
    ss << "\tn" << nodePos << " [label = \"" << tree->data.data << "\"];\n";
    int currPos = nodePos;
    genStream(tree->left, ss, currPos);
    genStream(tree->right, ss, currPos);
    
}

void toSVG(ExpressionTree::Node* tree, std::string filename)
{
    std::stringstream ss;
    ss << "graph G {\n";
    genStream(tree, ss);
    ss << "}\n";
    std::ofstream file;
    file.open ("temp.dot");
    file << ss.str();
    file.close();
    std::string cmd = "dot -Tsvg temp.dot -o " + filename;
    system(cmd.c_str());
    system("rm temp.dot");
}