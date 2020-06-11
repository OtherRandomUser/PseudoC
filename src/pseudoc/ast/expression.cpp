#include <pseudoc/ast/expression.hpp>

#include <iostream>

using namespace ast;

I32Constant::I32Constant(int value)
{
    _value = value;
}

std::string I32Constant::print()
{
    return std::to_string(_value);
}

std::unique_ptr<irl::IrlSegment> I32Constant::code_gen()
{
    auto segment = std::make_unique<irl::IrlSegment>();
    auto literal = std::make_unique<irl::IntLiteral>();

    literal->tp = irl::LlvmAtomic::i32;
    literal->value = _value;

    segment->out_value = std::move(literal);

    return segment;
}

F32Constant::F32Constant(float value)
{
    _value = value;
}

std::string F32Constant::print()
{
    return std::to_string(_value);
}

std::unique_ptr<irl::IrlSegment> F32Constant::code_gen()
{
    // TODO
    return std::make_unique<irl::IrlSegment>();
}

VariableRef::VariableRef(std::string identifier)
{
    _identifier = identifier;
}

std::string VariableRef::print()
{
    return _identifier;
}

std::unique_ptr<irl::IrlSegment> VariableRef::code_gen()
{
    auto segment = std::make_unique<irl::IrlSegment>();

    auto ref = _var_scope->get_variable(_identifier);
    auto out = _var_scope->new_temp(ref->tp);

    segment->instructions.push_back(std::make_unique<irl::Load>(ref, out, 4));
    segment->out_value = std::move(out);

    return segment;
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

std::unique_ptr<irl::IrlSegment> Addition::code_gen()
{
    // TODO add instruction
    return std::make_unique<irl::IrlSegment>();
}

Subtraction::Subtraction(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs):
    BinaryOp(std::move(lhs), std::move(rhs))
{
}

std::string Subtraction::print()
{
    return "( " + _lhs->print() + " - " + _rhs->print() + " )";
}

std::unique_ptr<irl::IrlSegment> Subtraction::code_gen()
{
    // TODO sub instruction
    return std::make_unique<irl::IrlSegment>();
}

Multiplication::Multiplication(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs):
    BinaryOp(std::move(lhs), std::move(rhs))
{
}

std::string Multiplication::print()
{
    return "( " + _lhs->print() + " * " + _rhs->print() + " )";
}

std::unique_ptr<irl::IrlSegment> Multiplication::code_gen()
{
    // TODO mul instruction
    return std::make_unique<irl::IrlSegment>();
}

Division::Division(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs):
    BinaryOp(std::move(lhs), std::move(rhs))
{
}

std::string Division::print()
{
    return "( " + _lhs->print() + " / " + _rhs->print() + " )";
}

std::unique_ptr<irl::IrlSegment> Division::code_gen()
{
    // TODO div instruction
    return std::make_unique<irl::IrlSegment>();
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

std::unique_ptr<irl::IrlSegment> RegularAssignment::code_gen()
{
    // TODO use store instruction
    return std::make_unique<irl::IrlSegment>();
}