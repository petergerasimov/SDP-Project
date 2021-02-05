#include <iostream>
#include <cassert>
#include "../interpreter.hpp"

//Generate SVG from tree
void genStream(ExpressionTree::Node* tree, std::stringstream& ss, int prevPos = -1);
void toSVG(ExpressionTree::Node* tree, std::string filename);
std::vector<int> runFileFromMain(std::string filename, std::string inputs="");

int main()
{
    Interpreter i;
    
    //Testing expressions
    int res;
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

    //Testing variables
    assert(runFileFromMain("variableTest.pep")[0] == 12345);
    //Testing ifs/ nested ifs
    assert(runFileFromMain("ifTest.pep")[0] == 10);
    //Testing else
    assert(runFileFromMain("elseTest.pep")[0] == 16);
    //Testing goto
    assert(runFileFromMain("gotoTest.pep")[0] == 1337);
    //Testing while
    std::vector<int> outputs = runFileFromMain("whileTest.pep");
    for(int i = 0; i < 10; i++)
    {
        assert(outputs[i] == i);
    }
    //Testing nested loops
    outputs = runFileFromMain("whileTest.pep");
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < i; j++)
        {
            assert(outputs[j] == j);
        }
    }
    //Testing arrays
    outputs = runFileFromMain("arrayTest.pep");
    for(int i = 0; i < 10; i++)
    {
        assert(outputs[i] == i + 10);
    }

    //Testing scopes
    std::string filename = "scopeTest.pep";
    std::vector<Token> tokens = i.parser.parseFile(filename);
    try
    {
        i.interpretTokens(tokens);
    }
    catch(const std::exception& e)
    {
        assert(!strcmp(e.what(), "Variable a not defined."));
    }

    ExpressionTree::Node* tree = i.expTreeHelper.generate("((5+5) + 5)-(a*(2+5))");
    toSVG(tree, "nodes.svg");
    tree = i.optimizeTree(tree);
    toSVG(tree, "optimized.svg");
    std::cout << "No errors" << std::endl;
    return EXIT_SUCCESS;
}

void genStream(ExpressionTree::Node* tree, std::stringstream& ss, int prevPos)
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

std::vector<int> runFileFromMain(std::string filename, std::string inputs)
{
    //Pipe output from main program to outputs.txt
    std::string cmd;
    if(!inputs.empty())
    {
        cmd = inputs + " > ../a.out " + filename + " > outputs.txt";
    }
    else
    {
        cmd = "../a.out " + filename + " > outputs.txt";
    }
    system(cmd.c_str());
    //Open outputs.txt
    std::ifstream file("outputs.txt");
    //Values to return
    std::vector<int> outputs;
    while (!file.eof())
    {
        int tmp;
        file >> tmp;
        outputs.push_back(tmp);
    }
    file.close();
    system("rm outputs.txt");
    return outputs;
}