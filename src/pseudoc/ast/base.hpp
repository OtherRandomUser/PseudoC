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
        virtual std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) = 0;

        virtual void set_variable_scope(std::shared_ptr<VariableScope> var_scope, std::shared_ptr<FunctionTable> ftable)
        {
            _var_scope = std::move(var_scope);
            _ftable = std::move(ftable);
        }

        irl::LlvmAtomic get_type()
        {
            return _tp;
        }

    protected:
        std::shared_ptr<VariableScope> _var_scope;
        std::shared_ptr<FunctionTable> _ftable;
        irl::LlvmAtomic _tp;
    };
}