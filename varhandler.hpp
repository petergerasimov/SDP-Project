#ifndef __VARHANDLER_HPP__
#define __VARHANDLER_HPP__

#include <string>
#include <unordered_map>

struct Variable
{
public:
    std::string name;
    bool isArray = false;
    Variable();
    ~Variable();
    bool operator==(const Variable &rhs) const
    {
        return (name == rhs.name);
    }
    //
    int getValue(size_t index = 0);
    void invalidate();
    bool isValid();
    
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
    void add(std::string name, bool isArray = false, size_t sz = 0);
    void changeValue(std::string name, int value, size_t index);
    void getValue(std::string name, size_t index);
    void invalidate(std::string name);
    void isValid(std::string name);



private:
    std::unordered_map<std::string, Variable> varMap;
};

#endif //__VARHANDLER_HPP__