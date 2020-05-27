#include <pseudoc/variable-map.hpp>


void VariableScope::add_variable(std::string id, irl::LlvmType type)
{
    if (!_variables.empty())
    {
        auto it = _variables.find(id);

        if (it != _variables.end())
            throw std::logic_error("variable " + id + "redeclared");
    }

    _variables[id] = type;
}

irl::LlvmType VariableScope::get_variable_type(const std::string& id)
{
    auto it = _variables.find(id);

    if (it == _variables.end())
    {
        if (_parent)
            return _parent->get_variable_type(id);

        throw std::logic_error("reference to undeclared variable " + id);
    }

    return it->second;
}

TempVariableGenerator::TempVariableGenerator()
{
    _current = 0;
}

void TempVariableGenerator::set_variable_scope(std::shared_ptr<VariableScope> scope)
{
    _scope = std::move(scope);
}

std::shared_ptr<irl::Variable> TempVariableGenerator::get_ref_value(const std::string& id)
{
    auto it = _refs.find(id);

    if (it != _refs.end())
        return it->second;

    auto tp = _scope->get_variable_type(id);
    tp.ptr_level++;

    auto var = create_temporary(tp);
    _refs[id] = var;

    return var;
}

std::shared_ptr<irl::Variable> TempVariableGenerator::create_temporary(irl::LlvmType tp)
{
    auto var = std::make_shared<irl::Variable>();
    var->id = '\%' + std::to_string(_current++);
    var->tp = tp;

    return var;
}