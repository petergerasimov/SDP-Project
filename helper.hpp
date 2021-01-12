#ifndef __HELPER_HPP__
#define __HELPER_HPP__

#include <string>

enum keyWord {LET, READ, PRINT, WHILE, DONE, IF, ELSE, ENDIF, ASSIGN, GOTO, LABEL, INT, OPERATOR};

struct Token
{
    keyWord keywrd;
    std::string data;
};


#endif //__HELPER_HPP__
