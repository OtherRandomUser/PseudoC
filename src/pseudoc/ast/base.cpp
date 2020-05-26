#include <pseudoc/ast/base.hpp>

using namespace ast;

void VariableScope::add_variable(std::string id, int type)
{
    // TODO check redeclaration
    _variables[id] = type;
}

int VariableScope::get_variable_type(const std::string& id)
{
    auto it = _variables.find(id);

    if (it == _variables.end())
        return 0;

    return it->second;
}

int SubordinateScope::get_variable_type(const std::string& id)
{
    auto it = _variables.find(id);

    if (it == _variables.end())
        return _parent->get_variable_type(id);

    return it->second;
}