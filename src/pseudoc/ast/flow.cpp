#include <pseudoc/ast/flow.hpp>

using namespace ast;

IfStatement::IfStatement(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> on_true):
    _condition(std::move(condition)),
    _on_true(std::move(on_true))
{
}

IfStatement::IfStatement(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> on_true, std::unique_ptr<Statement> on_false):
    _condition(std::move(condition)),
    _on_true(std::move(on_true)),
    _on_false(std::move(on_false))
{
}

std::string IfStatement::print()
{
    if (_on_false)
        return "if (" + _condition->print() + ")\nthen "
            + _on_true->print() + "else "
            + _on_false->print();
    else
        return "if (" + _condition->print() + ")\nthen "
            + _on_true->print();
}

std::unique_ptr<irl::IrlSegment> IfStatement::code_gen()
{
    auto segment = std::make_unique<irl::IrlSegment>();

    auto condition = _condition->code_gen();

    for (auto& i: condition->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    auto ref_true = _var_scope->new_temp(irl::LlvmAtomic::v);
    auto on_true = _on_true->code_gen();

    if (!_on_false)
    {
        auto ref_end = _var_scope->new_temp(irl::LlvmAtomic::v);
        auto label_end = std::make_unique<irl::Label>(ref_end);

        segment->instructions.push_back(std::make_unique<irl::JumpC>(std::move(condition->out_value), ref_true, ref_end));
        segment->instructions.push_back(std::make_unique<irl::Label>(std::move(ref_true)));

        for (auto& i: on_true->instructions)
        {
            segment->instructions.push_back(std::move(i));
        }

        segment->instructions.push_back(std::make_unique<irl::Jump>(ref_end));
        segment->instructions.push_back(std::make_unique<irl::Label>(std::move(ref_end)));

        return segment;
    }

    auto ref_false = _var_scope->new_temp(irl::LlvmAtomic::v);
    auto on_false = _on_false->code_gen();

    auto ref_end = _var_scope->new_temp(irl::LlvmAtomic::v);

    segment->instructions.push_back(std::make_unique<irl::JumpC>(std::move(condition->out_value), ref_true, ref_false));
    segment->instructions.push_back(std::make_unique<irl::Label>(std::move(ref_true)));

    for (auto& i: on_true->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    segment->instructions.push_back(std::make_unique<irl::Jump>(ref_end));
    segment->instructions.push_back(std::make_unique<irl::Label>(std::move(ref_false)));

    for (auto& i: on_false->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    segment->instructions.push_back(std::make_unique<irl::Jump>(ref_end));
    segment->instructions.push_back(std::make_unique<irl::Label>(std::move(ref_end)));

    return segment;
}

void IfStatement::set_variable_scope(std::shared_ptr<VariableScope> var_scope, std::shared_ptr<FunctionTable> ftable)
{
    _condition->set_variable_scope(var_scope, ftable);
    _on_true->set_variable_scope(var_scope, ftable);

    if (_on_false)
        _on_false->set_variable_scope(var_scope, ftable);

    _var_scope = std::move(var_scope);
    _ftable = std::move(ftable);
}
