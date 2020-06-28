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
        virtual std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override = 0;

        virtual void set_variable_scope(std::shared_ptr<VariableScope> var_scope, std::shared_ptr<FunctionTable> ftable) override
        {
            _var_scope = std::move(var_scope);
            _ftable = std::move(ftable);
        }
    };

    class VariableDeclaration : public AstNode
    {
    public:
        VariableDeclaration(std::string identifier, std::unique_ptr<Expression> initializer);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override;

        void set_variable_scope(std::shared_ptr<VariableScope> var_scope, std::shared_ptr<FunctionTable> ftable) override
        {
            if (_initializer)
                _initializer->set_variable_scope(var_scope, _ftable);

            _var_scope = std::move(var_scope);
            _ftable = std::move(ftable);
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
        std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override;

        void set_variable_scope(std::shared_ptr<VariableScope> var_scope, std::shared_ptr<FunctionTable> ftable) override
        {
            for (auto& decl: _decls)
            {
                decl->set_variable_scope(var_scope, _ftable);
            }

            _var_scope = std::move(var_scope);
            _ftable = std::move(ftable);
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
        std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override;

        void set_variable_scope(std::shared_ptr<VariableScope> var_scope, std::shared_ptr<FunctionTable> ftable) override
        {
            _expr->set_variable_scope(var_scope, _ftable);
            _var_scope = std::move(var_scope);
            _ftable = std::move(ftable);
        }

    private:
        std::unique_ptr<Expression> _expr;
    };

    class ReturnStatement : public Statement
    {
    public:
        ReturnStatement(std::unique_ptr<Expression> expr);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override;

        void set_variable_scope(std::shared_ptr<VariableScope> var_scope, std::shared_ptr<FunctionTable> ftable) override
        {
            _expr->set_variable_scope(var_scope, _ftable);
            _var_scope = std::move(var_scope);
            _ftable = std::move(ftable);
        }

    private:
        std::unique_ptr<Expression> _expr;
    };

    class CompoundStatement : public Statement
    {
    public:
        void add_statement(std::unique_ptr<Statement> statement);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override;

        void forward_variable_scope(std::shared_ptr<VariableScope> var_scope, std::shared_ptr<FunctionTable> ftable)
        {
            _var_scope = std::move(var_scope);
            _ftable = std::move(ftable);

            for (auto& statement: _statements)
                statement->set_variable_scope(_var_scope, _ftable);
        }

        void set_variable_scope(std::shared_ptr<VariableScope> var_scope, std::shared_ptr<FunctionTable> ftable) override
        {
            forward_variable_scope(std::make_shared<VariableScope>(std::move(var_scope)), std::move(ftable));
        }

    private:
        std::vector<std::unique_ptr<Statement>> _statements;
    };
}