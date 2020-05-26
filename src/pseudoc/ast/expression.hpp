#pragma once

#include <memory>
#include <string>

#include <pseudoc/ast/base.hpp>

namespace ast
{
    class Expression : public AstNode
    {
    public:
        Expression(std::shared_ptr<VariableScope> scope);
        virtual ~Expression() = default;

        virtual std::string print() = 0;
    };

    class I32Constant : public Expression
    {
    public:
        I32Constant(std::shared_ptr<VariableScope> scope, int value);

        std::string print();

    private:
        int _value;
    };

    class F32Constant : public Expression
    {
    public:
        F32Constant(std::shared_ptr<VariableScope> scope, float value);

        std::string print();

    private:
        float _value;
    };

    class Identifier : public Expression
    {
    public:
        Identifier(std::shared_ptr<VariableScope> scope, std::string identifier);

        std::string print();

    private:
        std::string _identifier;
    };

    class BinaryOp : public Expression
    {
    public:
        BinaryOp(std::shared_ptr<VariableScope> scope, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

        virtual std::string print() = 0;
        
    protected:
        std::unique_ptr<Expression> _lhs;
        std::unique_ptr<Expression> _rhs;
    };

    class Addition : public BinaryOp
    {
    public:
        Addition(std::shared_ptr<VariableScope> scope, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

        std::string print();
    };

    class Subtraction : public BinaryOp
    {
    public:
        Subtraction(std::shared_ptr<VariableScope> scope, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

        std::string print();
    };

    class Multiplication : public BinaryOp
    {
    public:
        Multiplication(std::shared_ptr<VariableScope> scope, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

        std::string print();
    };

    class Division : public BinaryOp
    {
    public:
        Division(std::shared_ptr<VariableScope> scope, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

        std::string print();
    };
}