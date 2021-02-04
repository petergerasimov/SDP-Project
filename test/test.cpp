#include <iostream>
#include <cassert>
#include "../interpreter.hpp"

int main()
{
    Interpreter i;
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
    res = i.evaluateExpression("5 + 5*5/7");
    assert(res == (5 + 5*5/7));
    res = i.evaluateExpression("-1");
    assert(res == (-1));
    
    return EXIT_SUCCESS;
}