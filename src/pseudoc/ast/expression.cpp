#include <pseudoc/ast/expression.hpp>

using namespace ast;

I32Constant::I32Constant(int value)
{
    _value = value;
}

std::string I32Constant::print()
{
    return std::to_string(_value);
}

irl::IrlSegment I32Constant::code_gen()
{
    return irl::IrlSegment();
}

F32Constant::F32Constant(float value)
{
    _value = value;
}

std::string F32Constant::print()
{
    return std::to_string(_value);
}

irl::IrlSegment F32Constant::code_gen()
{
    return irl::IrlSegment();
}

Identifier::Identifier(std::string identifier)
{
    _identifier = identifier;
}

std::string Identifier::print()
{
    return _identifier;
}

irl::IrlSegment Identifier::code_gen()
{
    return irl::IrlSegment();
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

irl::IrlSegment Addition::code_gen()
{
    return irl::IrlSegment();
}

Subtraction::Subtraction(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs):
    BinaryOp(std::move(lhs), std::move(rhs))
{
}

std::string Subtraction::print()
{
    return "( " + _lhs->print() + " - " + _rhs->print() + " )";
}

irl::IrlSegment Subtraction::code_gen()
{
    return irl::IrlSegment();
}

Multiplication::Multiplication(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs):
    BinaryOp(std::move(lhs), std::move(rhs))
{
}

std::string Multiplication::print()
{
    return "( " + _lhs->print() + " * " + _rhs->print() + " )";
}

irl::IrlSegment Multiplication::code_gen()
{
    return irl::IrlSegment();
}

Division::Division(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs):
    BinaryOp(std::move(lhs), std::move(rhs))
{
}

std::string Division::print()
{
    return "( " + _lhs->print() + " / " + _rhs->print() + " )";
}

irl::IrlSegment Division::code_gen()
{
    return irl::IrlSegment();
}

AssignmentExpression::AssignmentExpression(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs):
    _lhs(std::move(lhs)),
    _rhs(std::move(rhs))
{
}

RegularAssignment::RegularAssignment(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs):
    AssignmentExpression(std::move(lhs), std::move(rhs))
{
}

std::string RegularAssignment::print()
{
    return "( " + _lhs->print() + " = " + _rhs->print() + " )";
}

irl::IrlSegment RegularAssignment::code_gen()
{
    return irl::IrlSegment();
}