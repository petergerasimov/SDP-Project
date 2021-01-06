#include "parser.hpp"

#include <fstream>

void Parser::parseFile(std::string filename)
{
    std::ifstream file(filename);
    if(!file)
    {
        return;
    }
    file >>
}
void Parser::parseString(std::string str)
{

}