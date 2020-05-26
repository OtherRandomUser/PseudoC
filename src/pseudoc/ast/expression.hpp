#pragma once

#include <memory>
#include <string>

#include <pseudoc/ast/base.hpp>

namespace ast
{
    class Expression : public AstNode
    {
    public:
        virtual ~Expression() = default;

        virtual std::string print() = 0;
        virtual irl::IrlSegment code_gen() = 0;
    };

    class I32Constant : public Expression
    {
    public:
        I32Constant(int value);

        std::string print() override;
        irl::IrlSegment code_gen() override;

    private:
        int _value;
    };

    class F32Constant : public Expression
    {
    public:
        F32Constant(float value);

        std::string print() override;
        irl::IrlSegment code_gen() override;

    private:
        float _value;
    };

    class Identifier : public Expression
    {
    public:
        Identifier(std::string identifier);

        std::string print() override;
        irl::IrlSegment code_gen() override;

    private:
        std::string _identifier;
    };

    class BinaryOp : public Expression
    {
    public:
        BinaryOp(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

        virtual std::string print() = 0;
        virtual irl::IrlSegment code_gen() = 0;
        
    protected:
        std::unique_ptr<Expression> _lhs;
        std::unique_ptr<Expression> _rhs;
    };

    class Addition : public BinaryOp
    {
    public:
        Addition(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

        std::string print() override;
        irl::IrlSegment code_gen() override;
    };

    class Subtraction : public BinaryOp
    {
    public:
        Subtraction(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

        std::string print() override;
        irl::IrlSegment code_gen() override;
    };

    class Multiplication : public BinaryOp
    {
    public:
        Multiplication(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

        std::string print() override;
        irl::IrlSegment code_gen() override;
    };

    class Division : public BinaryOp
    {
    public:
        Division(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

        std::string print() override;
        irl::IrlSegment code_gen() override;
    };

    class AssignmentExpression : public Expression
    {
    public:
        AssignmentExpression(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

        virtual std::string print() = 0;
        virtual irl::IrlSegment code_gen() = 0;
        
    protected:
        std::unique_ptr<Expression> _lhs;
        std::unique_ptr<Expression> _rhs;
    };

    class RegularAssignment : public AssignmentExpression
    {
    public:
        RegularAssignment(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

        std::string print() override;
        irl::IrlSegment code_gen() override;
    };
}