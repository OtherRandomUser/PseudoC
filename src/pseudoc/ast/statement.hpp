#pragma once

#include <vector>

#include <pseudoc/ast/base.hpp>
#include <pseudoc/ast/expression.hpp>

namespace ast
{
    class Statement : public AstNode
    {
    public:
        virtual ~Statement() = default;

        virtual std::string print() override = 0;
        virtual std::unique_ptr<irl::IrlSegment> code_gen() override = 0;

        virtual void set_variable_scope(std::shared_ptr<VariableScope> var_scope) override
        {
            _var_scope = std::move(var_scope);
        }
    };

    class VariableDeclaration : public AstNode
    {
    public:
        VariableDeclaration(std::string identifier, std::unique_ptr<Expression> initializer);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen() override;

        void set_variable_scope(std::shared_ptr<VariableScope> var_scope) override
        {
            if (_initializer)
                _initializer->set_variable_scope(var_scope);

            _var_scope = std::move(var_scope);
        }

    private:
        std::string _identifier;
        std::unique_ptr<Expression> _initializer;
    };

    class DeclarationStatement : public Statement
    {
    public:
        DeclarationStatement(std::unique_ptr<VariableDeclaration> decl);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen() override;

        void set_variable_scope(std::shared_ptr<VariableScope> var_scope) override
        {
            for (auto& decl: _decls)
            {
                decl->set_variable_scope(var_scope);
            }

            _var_scope = std::move(var_scope);
        }

        void add_variable(std::unique_ptr<VariableDeclaration> decl);
    
    private:
        std::vector<std::unique_ptr<VariableDeclaration>> _decls;
    };

    class ExpressionStatement : public Statement
    {
    public:
        ExpressionStatement(std::unique_ptr<Expression> expr);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen() override;

        void set_variable_scope(std::shared_ptr<VariableScope> var_scope) override
        {
            _expr->set_variable_scope(var_scope);
            _var_scope = std::move(var_scope);
        }

    private:
        std::unique_ptr<Expression> _expr;
    };

    class CompoundStatement : public Statement
    {
    public:
        void add_statement(std::unique_ptr<Statement> statement);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen() override;

        void set_variable_scope(std::shared_ptr<VariableScope> var_scope) override
        {
            _var_scope = std::make_shared<VariableScope>(std::move(var_scope));

            for (auto& statement: _statements)
                statement->set_variable_scope(_var_scope);
        }

    private:
        std::vector<std::unique_ptr<Statement>> _statements;
    };
}