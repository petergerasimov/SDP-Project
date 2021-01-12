#include "varhandler.hpp"

Variable::Variable(const std::string& name, size_t sz)
{
    this->name = name;
    this->sz = sz;
    isArray = (sz > 1);
    if(isArray)
    {
        makeArray(sz);
    }
    else
    {
        makeVariable();
    }
    
}
Variable::Variable(const Variable &rhs)
{
    name = rhs.name;
    isArray = rhs.isArray;
    isValid = rhs.isValid;
    sz = rhs.sz;
    pointer = new int[sz];
    *pointer = *rhs.pointer;
}
Variable::~Variable()
{
    if(pointer != nullptr)
    {
        delete[] pointer;
    }
}
bool Variable::operator==(const Variable &rhs) const
{
    return (name == rhs.name);
}
//
void Variable::makeArray(size_t& sz)
{
    isArray = true;
    isValid = true;
    int* newArr = new int[sz];
    //Initialize with 0s
    //The interpreter should't allow conversion from
    //variable to array or resizing of an array
    for(int i = 0; i < sz; i++)
    {
        newArr[i] = 0;
    }
    if(pointer != nullptr)
    {   
        delete[] pointer;
    }
    pointer = newArr;
}
void Variable::makeVariable()
{
    isArray = false;
    isValid = true;
    int* newVar = new int[1];
    if(pointer != nullptr)
    {   
        delete[] pointer;
    }
    pointer = newVar;
}
void Variable::setValue(int& value, size_t index)
{
    if (getValidity())
    {
        if (isArray)
        {
            pointer[index] = value;
        }
        else
        {
            pointer[0] = value;
            if (index > 0)
            {
                throw std::runtime_error(name + " is not an array");
            }
        }
    }
    else
    {
        throw std::runtime_error(name + " is not defined in this scope");
    }
    
}
//
const int Variable::getValue(size_t index) const
{
    if (getValidity())
    {
        if (isArray)
        {
            return pointer[index];
        }
        else
        {
            return pointer[0];
            if (index > 0)
            {
                throw std::runtime_error(name + " is not an array");
            }
        }
    }
    else
    {
        throw std::runtime_error(name + " is not defined in this scope");
    }
}
void Variable::invalidate()
{
    if(pointer != nullptr)
    {
        delete[] pointer;
    }
    pointer = nullptr;
    isValid = false;
}
bool Variable::getValidity() const
{
    return isValid;
}


//Variable handler
typedef std::unordered_map<std::string, Variable>::iterator varMapIter;

void VarHandler::add(const std::string& name, size_t sz)
{
    varMapIter it = varMap.find(name);
    if(it != varMap.end())
    {
        if(it->second.getValidity())
        {
            throw std::runtime_error("Multiple declarations of variable " + name);
        }
        else
        {
            if(sz <= 1)
            {
                it->second.makeVariable();
            }
            else
            {
                it->second.makeArray(sz);
            }
            
        }
    }
    else
    {
        Variable tmp(name, sz);
        varMap.insert({name, tmp});
    }
}
void VarHandler::changeValue(const std::string& name, int& value, size_t index)
{
    varMapIter it = varMap.find(name);
    if(it != varMap.end())
    {
        if(it->second.getValidity())
        {
            it->second.setValue(value, index);
        }
        else
        {
            throw std::runtime_error("Variable " + name + " not defined.");
        }
    }
    else
    {
        throw std::runtime_error("Variable " + name + " not defined.");
    }
}
int VarHandler::getValue(const std::string& name, size_t index)
{
    varMapIter it = varMap.find(name);
    if(it != varMap.end())
    {
        if(it->second.getValidity())
        {
            return it->second.getValue(index);
        }
        else
        {
            throw std::runtime_error("Variable " + name + " not defined.");
        }
    }
    else
    {
        throw std::runtime_error("Variable " + name + " not defined.");
    }
    
    return 0;
}
void VarHandler::invalidate(const std::string& name)
{
    varMapIter it = varMap.find(name);
    if(it != varMap.end())
    {
        it->second.invalidate();
    }
}
bool VarHandler::isValid(const std::string& name)
{
    varMapIter it = varMap.find(name);
    if(it != varMap.end())
    {
        return it->second.getValidity();
    }
    return false;
}