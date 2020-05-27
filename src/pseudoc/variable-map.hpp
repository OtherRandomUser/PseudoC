#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <pseudoc/irl/type.hpp>
#include <pseudoc/irl/value.hpp>

class VariableScope
{
public:
    VariableScope() = default;

    VariableScope(std::shared_ptr<VariableScope> parent):
        _parent(std::move(parent))
    {
        _variables.reserve(5);
    }

    void add_variable(std::string id, irl::LlvmType type_id);
    irl::LlvmType get_variable_type(const std::string& id);

protected:
    std::shared_ptr<VariableScope> _parent;
    std::unordered_map<std::string, irl::LlvmType> _variables;
};

class TempVariableGenerator
{
public:
    TempVariableGenerator();

    void set_variable_scope(std::shared_ptr<VariableScope> scope);

    std::shared_ptr<irl::Variable> get_ref_value(const std::string& id);
    std::shared_ptr<irl::Variable> create_temporary(irl::LlvmType tp);

private:
    std::shared_ptr<VariableScope> _scope;
    std::unordered_map<std::string, std::shared_ptr<irl::Variable>> _refs;

    int _current;
};