#include <iostream>

// Да се напише интерпретатор на езика, определен със запазените думи:
// LАBEL, GOTO, LET, READ, PRINT, IF, ENDIF, ELSE, WHILE, DONE
// И оператори =, ==, !=, <, <=, >, >=, +, *, /, -, %, &&, ||, !,()
// работещ над целочислени данни и масиви от такива. Масивите се дефинират с конкретен размер. Операторите имат синтаксис и семантика, идентични със C++, като можете да построявате произволно сложни изрази.
// Вашият интерпретатор трябва да може да работи над подадени текстови файлове, както и в REPL режим. Проверявайте за синтактични грешки и за всички такива изведете подходящи съобщения.

// С LABEL се задава етикет за безусловен преход адресиран с GOTO. Може да се разполага на произволно място в програмата.
// С LET се въвежда променлива. Всяка променлива трябва да бъде обявена преди да се използва. Областта на действие е от декларацията до края на блока.
// READ и PRINT се използват за четене на стойности от стандартния вход и извеждане на екрана на стойност.
// Конструкцията за условен преход IF има следния синтаксис:
// IF <булев или целочислен израз> <блок код> [ELSE <блок код>] ENDIF
// Конструкцията за цикъл WHILE има следния синтаксис:
// WHILE <условие> <блок код> DONE

// Като примери за използване на вашия интерпретатор напишете на този език програма, която въвежда числа и намира най-малкото от тях.

// Пример за програма, която намира сума на определен брой числа:

#include "interpreter.hpp"
#include "expressiontree.hpp"

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
    // system("rm temp.dot");
}

int main()
{
    Interpreter i;
    std::string file = "test1.txt";
    std::vector<Token> tokens = i.parser.parseFile(file);
    
    
    static const std::map<int, std::string> testMap = {
        {LET, "LET"},
        {READ, "READ"},
        {PRINT, "PRINT"},
        {WHILE, "WHILE"},
        {DONE, "DONE"},
        {IF, "IF"},
        {ELSE, "ELSE"},
        {ENDIF, "ENDIF"},
        {ASSIGN, "ASSIGN"},
        {GOTO, "GOTO"},
        {LABEL, "LABEL"},
        {INT, "INT"},
        {VAR, "VAR"},
        {OPERATOR, "OPERATOR"}
    };

    std::vector<Token> toes = i.parser.parseExpression("count = 1 == 5 + 3");
    for(auto& t : toes)
    {
        auto it = testMap.find(t.keywrd);
        if(it != testMap.end())
        {
            std::cout << it->second << " ";
        } 
        std::cout << t.data << std::endl;
    }
    std::cout << "HERE" << std::endl;
    ExpressionTree ex;
    toSVG(ex.generate("count < 0"), "nodes.svg");
    // for(auto& t : tokens)
    // {
    //     auto it = testMap.find(t.keywrd);
    //     if(it != testMap.end())
    //     {
    //         std::cout << it->second << " ";
    //     } 
    //     std::cout << t.data << std::endl;
    // }

    try
    {
        i.interpretTokens(tokens);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return EXIT_SUCCESS;
}