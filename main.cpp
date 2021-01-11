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

int main()
{
    Interpreter i;
    //"-((3 * (2 + 1) + 2) + 1)"; this is not working properly
    std::string s = "((3 > (2 == 1) + 2) + 1)";
    int res = 0;

    // std::vector<Token> tokens = i.parser.parseExpression(s);
    // for(const Token& t : tokens)
    // {
    //     std::cout << t.data << " " << (t.keywrd == INT ? "INT" : "OP")  << std::endl;
    // }
    std::string file = "test.txt";
    std::vector<Token> tokens = i.parser.parseFile(file);

    static const std::map<int, std::string> testMap = {
        {INT, "INT"},
        {OPERATOR, "OPERATOR"},
        {LET, "LET"},
        {READ, "READ"},
        {PRINT, "PRINT"},
        {GOTO, "GOTO"},
        {LABEL, "LABEL"},
        {WHILE, "WHILE"},
        {DONE, "DONE"},
        {IF, "IF"},
        {ELSE, "ELSE"},
        {ENDIF, "ENDIF"},
        {ASSIGN, "ASSIGN"}};

    for(auto& t : tokens)
    {
        auto it = testMap.find(t.keywrd);
        if(it != testMap.end())
        {
            std::cout << it->second << " ";
        } 
        std::cout << t.data << std::endl;
    }

    try
    {
        res = i.evaluateExpression(s);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    std::cout << res << std::endl;
    return EXIT_SUCCESS;
}