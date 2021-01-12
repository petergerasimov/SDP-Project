#ifndef __VARHANDLER_HPP__
#define __VARHANDLER_HPP__

#include <string>
#include <unordered_map>
#include <stdexcept>

class Variable
{
public:
    Variable(std::string& name, size_t sz = 0);
    Variable(const Variable &rhs);
    ~Variable();
    //this is for the hash map
    bool operator==(const Variable &rhs) const;
    //Make array and make variable
    //Should be used only when there is 
    //Already a variable with the same name
    //but is invalid
    void makeArray(size_t& sz);
    void makeVariable();
    //
    void setValue(int& value, size_t index = 0);
    //
    const int getValue(size_t index = 0) const;
    const std::string& getName() const;
    bool getValidity() const;
    //
    void invalidate();
    
    
private:
    std::string name;
    bool isArray;
    bool isValid = false;
    size_t sz;
    int *pointer = nullptr;
};

namespace std
{
    template <>
    struct hash<Variable>
    {
        std::size_t operator()(const Variable &var) const
        {
            return hash<std::string>()(var.getName());
        }
    };

}

class VarHandler
{
public:
    void add(std::string& name, size_t sz = 0);
    void changeValue(std::string& name, int& value, size_t index = 0);
    int getValue(std::string& name, size_t index = 0);
    void invalidate(std::string& name);
    bool isValid(std::string& name);
private:
    std::unordered_map<std::string, Variable> varMap;
};

#endif //__VARHANDLER_HPP__