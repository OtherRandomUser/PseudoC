#pragma once

#include <pseudoc/ast/base.hpp>
#include <pseudoc/ast/statement.hpp>

namespace ast
{
    class Definition : public AstNode
    {
    public:
        ~Definition() = default;

        virtual std::string print() override = 0;
        virtual std::unique_ptr<irl::IrlSegment> code_gen() override = 0;

        virtual void set_variable_scope(std::shared_ptr<VariableScope> var_scope, std::shared_ptr<FunctionTable> ftable) override
        {
            _var_scope = std::make_shared<VariableScope>();
            _ftable = std::move(ftable);
        }
    };

    class FunctionParam : public Definition
    {
    public:
        FunctionParam(std::string identifier, irl::LlvmAtomic tp);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen() override;

        void add_temp();

    private:
        std::string _identifier;
        std::shared_ptr<irl::Variable> _param_ref;
    };

    class FunctionDefinition : public Definition
    {
    public:
        FunctionDefinition(std::string identifier, irl::LlvmAtomic tp, std::unique_ptr<std::vector<std::unique_ptr<FunctionParam>>> params, std::unique_ptr<CompoundStatement> body);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen() override;

    private:
        std::string _identifier;
        std::vector<std::unique_ptr<FunctionParam>> _params;
        std::unique_ptr<CompoundStatement> _body;
    };
}