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

std::unique_ptr<irl::IrlSegment> I32Constant::code_gen(irl::Context context)
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

std::unique_ptr<irl::IrlSegment> F32Constant::code_gen(irl::Context context)
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

std::unique_ptr<irl::IrlSegment> VariableRef::code_gen(irl::Context context)
{
    auto segment = std::make_unique<irl::IrlSegment>();

    auto ref = _var_scope->get_variable(_identifier);
    auto out = _var_scope->new_temp(ref->tp);

    segment->instructions.push_back(std::make_unique<irl::Load>(ref, out, 4));
    segment->out_value = std::move(out);

    return segment;
}

PreIncrement::PreIncrement(std::string identifier, int value):
    _identifier(std::move(identifier))
{
    _value = value;
}

std::string PreIncrement::print()
{
    return "++(" + std::to_string(_value) + ") " + _identifier;
}

std::unique_ptr<irl::IrlSegment> PreIncrement::code_gen(irl::Context context)
{
    auto segment = std::make_unique<irl::IrlSegment>();

    auto ref = _var_scope->get_variable(_identifier);
    auto tp = ref->tp;
    auto ld_out = _var_scope->new_temp(tp);
    auto out = _var_scope->new_temp(tp);

    auto literal = std::make_unique<irl::IntLiteral>();

    literal->tp = tp;
    literal->value = _value;

    segment->instructions.push_back(std::make_unique<irl::Load>(ref, ld_out, 4));
    segment->instructions.push_back(std::make_unique<irl::Add>(out, ld_out, std::move(literal), tp));
    segment->instructions.push_back(std::make_unique<irl::Store>(out, std::move(ref), 4));
    segment->out_value = std::move(out);

    return segment;
}

PostIncrement::PostIncrement(std::string identifier, int value):
    _identifier(std::move(identifier))
{
    _value = value;
}

std::string PostIncrement::print()
{
    return _identifier + " ++(" + std::to_string(_value) + ")";
}

std::unique_ptr<irl::IrlSegment> PostIncrement::code_gen(irl::Context context)
{
    auto segment = std::make_unique<irl::IrlSegment>();

    auto ref = _var_scope->get_variable(_identifier);
    auto tp = ref->tp;
    auto out = _var_scope->new_temp(tp);
    auto inc_out = _var_scope->new_temp(tp);

    auto literal = std::make_unique<irl::IntLiteral>();

    literal->tp = tp;
    literal->value = _value;

    segment->instructions.push_back(std::make_unique<irl::Load>(ref, out, 4));
    segment->instructions.push_back(std::make_unique<irl::Add>(inc_out, out, std::move(literal), tp));
    segment->instructions.push_back(std::make_unique<irl::Store>(std::move(inc_out), std::move(ref), 4));
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

std::unique_ptr<irl::IrlSegment> Addition::code_gen(irl::Context context)
{
    auto segment = std::make_unique<irl::IrlSegment>();

    auto lhs = _lhs->code_gen(context);
    for (auto& i: lhs->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    auto rhs = _rhs->code_gen(std::move(context));
    for (auto& i: rhs->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    // TODO use node type
    auto tp = irl::LlvmAtomic::i32;
    auto out = _var_scope->new_temp(tp);
    segment->instructions.push_back(std::make_unique<irl::Add>(out, std::move(lhs->out_value), std::move(rhs->out_value), tp));
    segment->out_value = std::move(out);

    return segment;
}

Subtraction::Subtraction(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs):
    BinaryOp(std::move(lhs), std::move(rhs))
{
}

std::string Subtraction::print()
{
    return "( " + _lhs->print() + " - " + _rhs->print() + " )";
}

std::unique_ptr<irl::IrlSegment> Subtraction::code_gen(irl::Context context)
{
    auto segment = std::make_unique<irl::IrlSegment>();

    auto lhs = _lhs->code_gen(context);
    for (auto& i: lhs->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    auto rhs = _rhs->code_gen(std::move(context));
    for (auto& i: rhs->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    // TODO use node type
    auto tp = irl::LlvmAtomic::i32;
    auto out = _var_scope->new_temp(tp);
    segment->instructions.push_back(std::make_unique<irl::Sub>(out, std::move(lhs->out_value), std::move(rhs->out_value), tp));
    segment->out_value = std::move(out);

    return segment;
}

Multiplication::Multiplication(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs):
    BinaryOp(std::move(lhs), std::move(rhs))
{
}

std::string Multiplication::print()
{
    return "( " + _lhs->print() + " * " + _rhs->print() + " )";
}

std::unique_ptr<irl::IrlSegment> Multiplication::code_gen(irl::Context context)
{
    auto segment = std::make_unique<irl::IrlSegment>();

    auto lhs = _lhs->code_gen(context);
    for (auto& i: lhs->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    auto rhs = _rhs->code_gen(std::move(context));
    for (auto& i: rhs->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    // TODO use node type
    auto tp = irl::LlvmAtomic::i32;
    auto out = _var_scope->new_temp(tp);
    segment->instructions.push_back(std::make_unique<irl::Mul>(out, std::move(lhs->out_value), std::move(rhs->out_value), tp));
    segment->out_value = std::move(out);

    return segment;
}

Division::Division(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs):
    BinaryOp(std::move(lhs), std::move(rhs))
{
}

std::string Division::print()
{
    return "( " + _lhs->print() + " / " + _rhs->print() + " )";
}

std::unique_ptr<irl::IrlSegment> Division::code_gen(irl::Context context)
{
    auto segment = std::make_unique<irl::IrlSegment>();

    auto lhs = _lhs->code_gen(context);
    for (auto& i: lhs->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    auto rhs = _rhs->code_gen(std::move(context));
    for (auto& i: rhs->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    // TODO use node type
    auto tp = irl::LlvmAtomic::i32;
    auto out = _var_scope->new_temp(tp);
    segment->instructions.push_back(std::make_unique<irl::SDiv>(out, std::move(lhs->out_value), std::move(rhs->out_value), tp));
    segment->out_value = std::move(out);

    return segment;
}

Compare::Compare(Compare::Code code, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs):
    BinaryOp(std::move(lhs), std::move(rhs))
{
    _code = code;
    _tp = irl::LlvmAtomic::b;
}

std::string Compare::print()
{
    std::string code;

    switch (_code)
    {
    case Code::EQ:
        code = " == ";
        break;

    case Code::NE:
        code = " != ";
        break;

    case Code::LT:
        code = " < ";
        break;

    case Code::LE:
        code = " <= ";
        break;

    case Code::GT:
        code = " > ";
        break;

    case Code::GE:
        code = " >= ";
        break;
    }

    return "( " + _lhs->print() + code + _rhs->print() + " )";
}

std::unique_ptr<irl::IrlSegment> Compare::code_gen(irl::Context context)
{
    auto segment = std::make_unique<irl::IrlSegment>();

    auto lhs = _lhs->code_gen(context);
    for (auto& i: lhs->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    auto rhs = _rhs->code_gen(std::move(context));
    for (auto& i: rhs->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    irl::ICmp::CondT ct;

    switch (_code)
    {
    case Code::EQ:
        ct = irl::ICmp::CondT::eq;
        break;

    case Code::NE:
        ct = irl::ICmp::CondT::ne;
        break;

    case Code::LT:
        ct = irl::ICmp::CondT::slt;
        break;

    case Code::LE:
        ct = irl::ICmp::CondT::sle;
        break;

    case Code::GT:
        ct = irl::ICmp::CondT::sgt;
        break;

    case Code::GE:
        ct = irl::ICmp::CondT::sle;
        break;
    }

    // TODO use node type
    auto tp = irl::LlvmAtomic::i32;
    auto out = _var_scope->new_temp(_tp);
    segment->instructions.push_back(std::make_unique<irl::ICmp>(ct, out, std::move(lhs->out_value), std::move(rhs->out_value), tp));
    segment->out_value = std::move(out);

    return segment;
}

LogicalAnd::LogicalAnd(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs):
    BinaryOp(std::move(lhs), std::move(rhs))
{
}

std::string LogicalAnd::print()
{
    return "( " + _lhs->print() + " and " + _rhs->print() + " )";
}

std::unique_ptr<irl::IrlSegment> LogicalAnd::code_gen(irl::Context context)
{
    auto segment = std::make_unique<irl::IrlSegment>();

    auto lhs = _lhs->code_gen(context);
    for (auto& i: lhs->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    auto rhs = _rhs->code_gen(std::move(context));
    for (auto& i: rhs->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    // TODO solve this headache

    return segment;
}

LogicalOr::LogicalOr(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs):
    BinaryOp(std::move(lhs), std::move(rhs))
{
}

std::string LogicalOr::print()
{
    return "( " + _lhs->print() + " and " + _rhs->print() + " )";
}

std::unique_ptr<irl::IrlSegment> LogicalOr::code_gen(irl::Context context)
{
    auto segment = std::make_unique<irl::IrlSegment>();

    auto lhs = _lhs->code_gen(context);
    for (auto& i: lhs->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    auto rhs = _rhs->code_gen(std::move(context));
    for (auto& i: rhs->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    // TODO solve this headache

    return segment;
}

AssignmentExpression::AssignmentExpression(std::string identifier, std::unique_ptr<Expression> inner):
    _identifier(std::move(identifier)),
    _inner(std::move(inner))
{
}

RegularAssignment::RegularAssignment(std::string identifier, std::unique_ptr<Expression> inner):
    AssignmentExpression(std::move(identifier), std::move(inner))
{
}

std::string RegularAssignment::print()
{
    return "( " + _identifier + " = " + _inner->print() + " )";
}

std::unique_ptr<irl::IrlSegment> RegularAssignment::code_gen(irl::Context context)
{
    auto segment = std::make_unique<irl::IrlSegment>();

    auto ref = _var_scope->get_variable(_identifier);

    auto inner = _inner->code_gen(std::move(context));
    for (auto& i: inner->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    segment->instructions.push_back(std::make_unique<irl::Store>(inner->out_value, std::move(ref), 4));
    segment->out_value = std::move(inner->out_value);

    return segment;
}

BooleanCast::BooleanCast(std::unique_ptr<Expression> inner):
    _inner(std::move(inner))
{
    _tp = irl::LlvmAtomic::b;
}

std::string BooleanCast::print()
{
    return "(bool) " + _inner->print();
}

std::unique_ptr<irl::IrlSegment> BooleanCast::code_gen(irl::Context context)
{
    auto segment = std::make_unique<irl::IrlSegment>();
    auto inner = _inner->code_gen(std::move(context));
    auto ref = _var_scope->new_temp(irl::LlvmAtomic::b);

    for (auto& i: inner->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    auto zero = std::make_unique<irl::IntLiteral>();
    zero->tp = irl::LlvmAtomic::i32;
    zero->value = 0;

    segment->instructions.push_back(std::make_unique<irl::ICmp>(irl::ICmp::ne, ref, std::move(inner->out_value), std::move(zero), irl::LlvmAtomic::i32));

    segment->out_value = std::move(ref);
    return segment;
}

ConditionalExpression::ConditionalExpression(std::unique_ptr<Expression> condition, std::unique_ptr<Expression> true_branch, std::unique_ptr<Expression> false_branch):
    _condition(std::move(condition)),
    _true_branch(std::move(true_branch)),
    _false_branch(std::move(false_branch))
{
}

std::string ConditionalExpression::print()
{
    return "( " + _condition->print() + " ? " + _true_branch->print() + " : " + _false_branch->print() + " )";
}

std::unique_ptr<irl::IrlSegment> ConditionalExpression::code_gen(irl::Context context)
{
    auto segment = std::make_unique<irl::IrlSegment>();

    auto condition = _condition->code_gen(context);

    for (auto& i: condition->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    auto ref_true = _var_scope->new_temp(irl::LlvmAtomic::v);
    auto true_branch = _true_branch->code_gen(context);

    auto ref_false = _var_scope->new_temp(irl::LlvmAtomic::v);
    auto false_branch = _false_branch->code_gen(std::move(context));

    auto ref_end = _var_scope->new_temp(irl::LlvmAtomic::v);

    segment->instructions.push_back(std::make_unique<irl::JumpC>(std::move(condition->out_value), ref_true, ref_false));
    segment->instructions.push_back(std::make_unique<irl::Label>(ref_true));

    for (auto& i: true_branch->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    segment->instructions.push_back(std::make_unique<irl::Jump>(ref_end));
    segment->instructions.push_back(std::make_unique<irl::Label>(ref_false));

    for (auto& i:false_branch->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    segment->instructions.push_back(std::make_unique<irl::Jump>(ref_end));
    segment->instructions.push_back(std::make_unique<irl::Label>(std::move(ref_end)));

    // TODO use node type
    auto tp = irl::LlvmAtomic::i32;
    auto out = _var_scope->new_temp(tp);

    auto phi = std::make_unique<irl::Phi>(out, tp);
    phi->add_branch(std::move(true_branch->out_value), std::move(ref_true));
    phi->add_branch(std::move(false_branch->out_value), std::move(ref_false));

    segment->instructions.push_back(std::move(phi));
    segment->out_value = std::move(out);

    return segment;
}
