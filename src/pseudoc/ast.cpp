#include <pseudoc/ast.hpp>

I32Literal::I32Literal(int value)
{
    _value = value;
}

std::string I32Literal::print()
{
    return std::to_string(_value);
}

Identifier::Identifier(std::string identifier)
{
    _identifier = identifier;
}

std::string Identifier::print()
{
    return _identifier;
}

BinaryOp::BinaryOp(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs):
    _lhs(std::move(lhs)),
    _rhs(std::move(rhs))
{
}

Addition::Addition(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs):
    BinaryOp(std::move(lhs), std::move(rhs))
{
}

std::string Addition::print()
{
    return "( " + _lhs->print() + " + " + _rhs->print() + " )";
}

Subtraction::Subtraction(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs):
    BinaryOp(std::move(lhs), std::move(rhs))
{
}

std::string Subtraction::print()
{
    return "( " + _lhs->print() + " - " + _rhs->print() + " )";
}

Multiplication::Multiplication(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs):
    BinaryOp(std::move(lhs), std::move(rhs))
{
}

std::string Multiplication::print()
{
    return "( " + _lhs->print() + " * " + _rhs->print() + " )";
}

Division::Division(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs):
    BinaryOp(std::move(lhs), std::move(rhs))
{
}

std::string Division::print()
{
    return "( " + _lhs->print() + " / " + _rhs->print() + " )";
}