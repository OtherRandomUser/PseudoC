#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <pseudoc/irl.hpp>

namespace ast
{
    // TODO other types/find a better way of handling types
    enum AtomicType
    {
        none,
        i32,
        f32,
    };

    class VariableScope
    {
    public:
        void add_variable(std::string id, AtomicType type);
        virtual AtomicType get_variable(const std::string& id);

    protected:
        std::unordered_map<std::string, AtomicType> _variables;
    };

    class SubordinateScope : VariableScope
    {
    public:
        SubordinateScope(std::shared_ptr<VariableScope> parent):
            _parent(std::move(parent))
        {
        }

        AtomicType get_variable(const std::string& id) override;

    private:
        std::shared_ptr<VariableScope> _parent;
    };

    class AstNode
    {
    public:
        AstNode(std::shared_ptr<VariableScope> var_scope):
            _var_scope(std::move(var_scope))
        {
        }

        virtual ~AstNode() = default;

        virtual std::string print() = 0;
        virtual irl::IrlSegment code_gen() = 0;

    private:
        std::shared_ptr<VariableScope> _var_scope;
    };
}