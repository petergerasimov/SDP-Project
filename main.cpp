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
#include <iostream>
#include <cstring>

int main(int argc,char* argv[])
{
    Interpreter i;
    if(argc == 1)
    {
        std::cout << "Run with --help for more info." << std::endl;
    }
    else if(argc == 2) 
    { 
        if(!strcmp(argv[1], "--help"))
        {
            std::cout << "--repl to run in repl mode\n" <<
                         "or just type a file name to run the file." << std::endl;
        }
        else if(!strcmp(argv[1], "--repl"))
        {
            try
            {
                i.repl();
            }
            catch(const std::exception& e)
            {
                // Wanting user to see the errors
                std::cout << e.what() << std::endl;
            }
        }
        else
        {
            std::string filename = argv[1];
            try
            {
                std::vector<Token> tokens = i.parser.parseFile(filename);
                i.interpretTokens(tokens);
            }
            catch(const std::exception& e)
            {
                std::cout << e.what() << std::endl;
            }
        }
    }
    else
    {
        std::cout << "Too many arguments" <<
                    "Run with --help for more info." << std::endl;
    }
    return EXIT_SUCCESS;
}