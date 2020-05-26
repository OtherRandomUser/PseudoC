#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <pseudoc/irl.hpp>

namespace ast
{
    class VariableScope
    {
    public:
        void add_variable(std::string id, int type_id);
        virtual int get_variable_type(const std::string& id);

    protected:
        std::unordered_map<std::string, int> _variables;
    };

    class SubordinateScope : VariableScope
    {
    public:
        SubordinateScope(std::shared_ptr<VariableScope> parent):
            _parent(std::move(parent))
        {
        }

        int get_variable_type(const std::string& id) override;

    private:
        std::shared_ptr<VariableScope> _parent;
    };

    class AstNode
    {
    public:
        virtual ~AstNode() = default;

        // TODO typecheck and type table
        // virtual int pinpoint_type() = 0;
        // TODO check variables
        // virtual void check_variable_scope(std::shared_ptr<VariableScope> var_scope) = 0;

        virtual std::string print() = 0;
        virtual irl::IrlSegment code_gen() = 0;

    private:
        std::shared_ptr<VariableScope> _var_scope;
    };
}