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

        virtual std::string print() = 0;
        virtual irl::IrlSegment code_gen() = 0;
    };

    class VariableDeclaration : public AstNode
    {
    public:
        VariableDeclaration(std::string identifier, int type_id, std::unique_ptr<Expression> initializer);

        std::string print() override;
        irl::IrlSegment code_gen() override;

    private:
        std::string _identifier;
        int _type_id;
        std::unique_ptr<Expression> _initializer;
    };

    class DeclarationStatement : public Statement
    {
    public:
        DeclarationStatement(std::unique_ptr<VariableDeclaration> decl);

        std::string print() override;
        irl::IrlSegment code_gen() override;

        void add_variable(std::unique_ptr<VariableDeclaration> decl);
    
    private:
        std::vector<std::unique_ptr<VariableDeclaration>> _decls;
    };

    class ExpressionStatement : public Statement
    {
    public:
        ExpressionStatement(std::unique_ptr<Expression> expr);

        std::string print() override;
        irl::IrlSegment code_gen() override;

    private:
        std::unique_ptr<Expression> _expr;
    };

    class CompoundStatement : public Statement
    {
    public:
        void add_statement(std::unique_ptr<Statement> statement);

        std::string print() override;
        irl::IrlSegment code_gen() override;

    private:
        std::vector<std::unique_ptr<Statement>> _statements;
    };
}