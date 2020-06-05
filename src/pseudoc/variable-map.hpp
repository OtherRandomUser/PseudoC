#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <pseudoc/irl/type.hpp>
#include <pseudoc/irl/value.hpp>

class IrlNameGenerator
{
public:
    auto get_next()
    {
        return std::to_string(_current++);
    }

private:
    int _current = 0;
};

class VariableScope
{
public:
    VariableScope();
    VariableScope(std::shared_ptr<VariableScope> parent);

    std::shared_ptr<irl::Variable> add_variable(std::string id, irl::LlvmType type_id);
    std::shared_ptr<irl::Variable> get_variable(const std::string& id);

protected:
    std::shared_ptr<IrlNameGenerator> _name_gen;

private:
    std::shared_ptr<VariableScope> _parent;
    std::unordered_map<std::string, std::shared_ptr<irl::Variable>> _variables;
};
