#include <pseudoc/ast/expression.hpp>

using namespace ast;

I32Constant::I32Constant(std::shared_ptr<VariableScope> scope, int value):
    Expression(std::move(scope))
{
    _value = value;
}

std::string I32Constant::print()
{
    return std::to_string(_value);
}

F32Constant::F32Constant(std::shared_ptr<VariableScope> scope, float value):
    Expression(std::move(scope))
{
    _value = value;
}

std::string F32Constant::print()
{
    return std::to_string(_value);
}

Identifier::Identifier(std::shared_ptr<VariableScope> scope, std::string identifier):
    Expression(std::move(scope))
{
    _identifier = identifier;
}

std::string Identifier::print()
{
    return _identifier;
}

BinaryOp::BinaryOp(std::shared_ptr<VariableScope> scope, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs):
    Expression(std::move(scope)),
    _lhs(std::move(lhs)),
    _rhs(std::move(rhs))
{
}

Addition::Addition(std::shared_ptr<VariableScope> scope, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs):
    BinaryOp(std::move(scope), std::move(lhs), std::move(rhs))
{
}

std::string Addition::print()
{
    return "( " + _lhs->print() + " + " + _rhs->print() + " )";
}

Subtraction::Subtraction(std::shared_ptr<VariableScope> scope, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs):
    BinaryOp(std::move(scope), std::move(lhs), std::move(rhs))
{
}

std::string Subtraction::print()
{
    return "( " + _lhs->print() + " - " + _rhs->print() + " )";
}

Multiplication::Multiplication(std::shared_ptr<VariableScope> scope, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs):
    BinaryOp(std::move(scope), std::move(lhs), std::move(rhs))
{
}

std::string Multiplication::print()
{
    return "( " + _lhs->print() + " * " + _rhs->print() + " )";
}

Division::Division(std::shared_ptr<VariableScope> scope, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs):
    BinaryOp(std::move(scope), std::move(lhs), std::move(rhs))
{
}

std::string Division::print()
{
    return "( " + _lhs->print() + " / " + _rhs->print() + " )";
}