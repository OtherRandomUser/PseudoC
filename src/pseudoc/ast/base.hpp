#pragma once

#include <memory>
#include <string>

#include <pseudoc/irl/segment.hpp>
#include <pseudoc/variable-map.hpp>

namespace ast
{
    class AstNode
    {
    public:
        virtual ~AstNode() = default;

        // TODO typecheck and type table
        // virtual int pinpoint_type() = 0;

        virtual std::string print() = 0;
        virtual std::unique_ptr<irl::IrlSegment> code_gen() = 0;

        virtual void set_variable_scope(std::shared_ptr<VariableScope> var_scope)
        {
            _var_scope = std::move(var_scope);
        }

    protected:
        std::shared_ptr<VariableScope> _var_scope;
        // irl::LlvmAtomic _type;
    };
}