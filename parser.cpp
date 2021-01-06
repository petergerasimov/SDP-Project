#include "parser.hpp"

#include <fstream>

void Parser::parseFile(std::string filename)
{
    std::ifstream file(filename);
    if(!file)
    {
        return;
    }
    bool isPrevKeyword = false;
    while(!file.eof())
    {
        std::string tmp;
        file >> tmp;
        if(tmp == )
        {

        }
    }
}
void Parser::parseString(std::string str)
{

}
