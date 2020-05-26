#include <pseudoc/ast/base.hpp>

using namespace ast;

void VariableScope::add_variable(std::string id, AtomicType type)
{
    // TODO check redeclaration
    _variables[id] = type;
}

AtomicType VariableScope::get_variable(const std::string& id)
{
    auto it = _variables.find(id);

    if (it == _variables.end())
        return AtomicType::none;

    return it->second;
}

AtomicType SubordinateScope::get_variable(const std::string& id)
{
    auto it = _variables.find(id);

    if (it == _variables.end())
        return _parent->get_variable(id);

    return it->second;
}