#include <pseudoc/variable-map.hpp>

VariableScope::VariableScope()
{
    _name_gen = std::make_shared<IrlNameGenerator>();
}

VariableScope::VariableScope(std::shared_ptr<VariableScope> parent):
    _parent(std::move(parent)),
    _name_gen(parent->_name_gen)
{
}

std::shared_ptr<irl::Variable> VariableScope::add_variable(std::string id, irl::LlvmType type)
{
    // verifies redeclaration on the current scope
    // it should be possible to correctly override the parent scope
    if (!_variables.empty())
    {
        auto it = _variables.find(id);

        if (it != _variables.end())
            throw std::logic_error("variable " + id + "redeclared");
    }

    auto var = std::make_shared<irl::Variable>();
    var->id = '\%' + _name_gen->get_next();
    var->tp = type;

    _variables[id] = var;

    return var;
}

std::shared_ptr<irl::Variable> VariableScope::get_variable(const std::string& id)
{
    auto it = _variables.find(id);

    if (it == _variables.end())
    {
        if (_parent)
            return _parent->get_variable(id);

        throw std::logic_error("reference to undeclared variable " + id);
    }

    return it->second;
}