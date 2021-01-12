#ifndef __VARHANDLER_HPP__
#define __VARHANDLER_HPP__

#include <string>
#include <unordered_map>

struct Variable
{
public:
    std::string name;
    bool isArray = false;
    int getValue(size_t index = 0);
    void invalidate();
    bool isValid();
    bool operator==(const Variable &rhs) const
    {
        return (name == rhs.name);
    }
private:
    int *pointer = nullptr;
};

namespace std
{
    template <>
    struct hash<Variable>
    {
        std::size_t operator()(const Variable &var) const
        {
            return hash<std::string>()(var.name);
        }
    };

}

class VarHandler
{
public:
    void addVariable();

private:
    std::unordered_map<std::string, Variable> varMap;
};

#endif //__VARHANDLER_HPP__