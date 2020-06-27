#pragma once

#include <pseudoc/ast/statement.hpp>

namespace ast
{
    class IfStatement : public Statement
    {
    public:
        IfStatement(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> on_true);
        IfStatement(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> on_true, std::unique_ptr<Statement> on_false);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen() override;

        void set_variable_scope(std::shared_ptr<VariableScope> var_scope, std::shared_ptr<FunctionTable> ftable) override;

    private:
        std::unique_ptr<Expression> _condition;
        std::unique_ptr<Statement> _on_true;
        std::unique_ptr<Statement> _on_false;
    };
}