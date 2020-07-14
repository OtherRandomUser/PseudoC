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

        virtual std::string print() override = 0;
        virtual std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override = 0;

        virtual void set_variable_scope(std::shared_ptr<VariableScope> var_scope, std::shared_ptr<FunctionTable> ftable) override
        {
            _var_scope = std::move(var_scope);
            _ftable = std::move(ftable);
        }
    };

    class I32Constant : public Expression
    {
    public:
        I32Constant(int value);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override;

    private:
        int _value;
    };

    class F32Constant : public Expression
    {
    public:
        F32Constant(float value);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override;

    private:
        float _value;
    };

    class VariableRef : public Expression
    {
    public:
        VariableRef(std::string identifier);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override;

    private:
        std::string _identifier;
    };

    class PreIncrement : public Expression
    {
    public:
        PreIncrement(std::string identifier, int value);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override;

    private:
        std::string _identifier;
        int _value;
    };

    class PostIncrement : public Expression
    {
    public:
        PostIncrement(std::string identifier, int value);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override;

    private:
        std::string _identifier;
        int _value;
    };

    class BinaryOp : public Expression
    {
    public:
        BinaryOp(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

        virtual std::string print() override = 0;
        virtual std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override = 0;

        void set_variable_scope(std::shared_ptr<VariableScope> var_scope, std::shared_ptr<FunctionTable> ftable) override
        {
            _lhs->set_variable_scope(var_scope, ftable);
            _rhs->set_variable_scope(var_scope, ftable);

            _var_scope = std::move(var_scope);
            _ftable = std::move(ftable);
        }
        
    protected:
        std::unique_ptr<Expression> _lhs;
        std::unique_ptr<Expression> _rhs;
    };

    class Addition : public BinaryOp
    {
    public:
        Addition(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override;
    };

    class Subtraction : public BinaryOp
    {
    public:
        Subtraction(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override;
    };

    class Multiplication : public BinaryOp
    {
    public:
        Multiplication(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override;
    };

    class Division : public BinaryOp
    {
    public:
        Division(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override;
    };

    class Compare : public BinaryOp
    {
    public:
        enum Code
        {
            EQ,
            NE,
            LT,
            LE,
            GT,
            GE
        };

        Compare(Code code, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override;

    private:
        Code _code;
    };

    class LogicalAnd : public BinaryOp
    {
    public:
        LogicalAnd(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override;
    };

    class LogicalOr : public BinaryOp
    {
    public:
        LogicalOr(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override;
    };

    class AssignmentExpression : public Expression
    {
    public:
        AssignmentExpression(std::string identifier, std::unique_ptr<Expression> inner);

        virtual std::string print() override = 0;
        virtual std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override = 0;

        void set_variable_scope(std::shared_ptr<VariableScope> var_scope, std::shared_ptr<FunctionTable> ftable) override
        {
            _inner->set_variable_scope(var_scope, ftable);

            _var_scope = std::move(var_scope);
            _ftable = std::move(ftable);
        }

    protected:
        std::string _identifier;
        std::unique_ptr<Expression> _inner;
    };

    class RegularAssignment : public AssignmentExpression
    {
    public:
        RegularAssignment(std::string identifier, std::unique_ptr<Expression> inner);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override;
    };

    class BooleanCast : public Expression
    {
    public:
        BooleanCast(std::unique_ptr<Expression> inner);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override;

        void set_variable_scope(std::shared_ptr<VariableScope> var_scope, std::shared_ptr<FunctionTable> ftable) override
        {
            _inner->set_variable_scope(var_scope, ftable);

            _var_scope = std::move(var_scope);
            _ftable = std::move(ftable);
        }

    private:
        std::unique_ptr<Expression> _inner;
    };

    class ConditionalExpression : public Expression
    {
    public:
        ConditionalExpression(std::unique_ptr<Expression> condition, std::unique_ptr<Expression> true_branch, std::unique_ptr<Expression> false_branch);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override;

        void set_variable_scope(std::shared_ptr<VariableScope> var_scope, std::shared_ptr<FunctionTable> ftable) override
        {
            _condition->set_variable_scope(var_scope, ftable);
            _true_branch->set_variable_scope(var_scope, ftable);
            _false_branch->set_variable_scope(var_scope, ftable);

            _var_scope = std::move(var_scope);
            _ftable = std::move(ftable);
        }

    private:
        std::unique_ptr<Expression> _condition;
        std::unique_ptr<Expression> _true_branch;
        std::unique_ptr<Expression> _false_branch;
    };

    class FCall : public Expression
    {
    public:
        FCall(std::string id, std::vector<std::unique_ptr<Expression>> params);

        std::string print() override;
        std::unique_ptr<irl::IrlSegment> code_gen(irl::Context context) override;

        void set_variable_scope(std::shared_ptr<VariableScope> var_scope, std::shared_ptr<FunctionTable> ftable) override
        {
            for (auto& param: _params)
            {
                param->set_variable_scope(var_scope, ftable);
            }

            _var_scope = std::move(var_scope);
            _ftable = std::move(ftable);
        }

    private:
        std::string _id;
        std::vector<std::unique_ptr<Expression>> _params;
    };
}