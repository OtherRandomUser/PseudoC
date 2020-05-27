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

std::unique_ptr<irl::IrlSegment> I32Constant::code_gen(std::shared_ptr<TempVariableGenerator> temp_gen)
{
    auto segment = std::make_unique<irl::IrlSegment>();
    auto literal = std::make_unique<irl::IntLiteral>();

    literal->tp.atomic = irl::LlvmAtomic::i32;
    literal->tp.ptr_level = 0;
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

std::unique_ptr<irl::IrlSegment> F32Constant::code_gen(std::shared_ptr<TempVariableGenerator> temp_gen)
{
    // TODO
    return std::make_unique<irl::IrlSegment>();
}

Identifier::Identifier(std::string identifier)
{
    _identifier = identifier;
}

std::string Identifier::print()
{
    return _identifier;
}

std::unique_ptr<irl::IrlSegment> Identifier::code_gen(std::shared_ptr<TempVariableGenerator> temp_gen)
{
    // TODO load instruction
    return std::make_unique<irl::IrlSegment>();
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

std::unique_ptr<irl::IrlSegment> Addition::code_gen(std::shared_ptr<TempVariableGenerator> temp_gen)
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

std::unique_ptr<irl::IrlSegment> Subtraction::code_gen(std::shared_ptr<TempVariableGenerator> temp_gen)
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

std::unique_ptr<irl::IrlSegment> Multiplication::code_gen(std::shared_ptr<TempVariableGenerator> temp_gen)
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

std::unique_ptr<irl::IrlSegment> Division::code_gen(std::shared_ptr<TempVariableGenerator> temp_gen)
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

std::unique_ptr<irl::IrlSegment> RegularAssignment::code_gen(std::shared_ptr<TempVariableGenerator> temp_gen)
{
    // TODO use store instruction
    return std::make_unique<irl::IrlSegment>();
}