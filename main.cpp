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