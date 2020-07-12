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

std::unique_ptr<irl::IrlSegment> IfStatement::code_gen(irl::Context context)
{
    auto segment = std::make_unique<irl::IrlSegment>();

    context.ph_true = _var_scope->new_placeholder(irl::LlvmAtomic::v);
    context.ph_false = _var_scope->new_placeholder(irl::LlvmAtomic::v);

    auto condition = _condition->code_gen(context);

    for (auto& i: condition->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    bool add_jump = segment->out_false != nullptr;

    // auto ref_true = _var_scope->new_temp(irl::LlvmAtomic::v);
    _var_scope->fix_placehoder(context.ph_true);
    auto on_true = _on_true->code_gen(context);

    // auto ref_false = _var_scope->new_temp(irl::LlvmAtomic::v);
    _var_scope->fix_placehoder(context.ph_false);

    if (!_on_false)
    {
        auto label_end = std::make_unique<irl::Label>(context.ph_false);

        if (add_jump)
            segment->instructions.push_back(std::make_unique<irl::JumpC>(std::move(condition->out_value), context.ph_false, context.ph_false));

        segment->instructions.push_back(std::make_unique<irl::Label>(std::move(context.ph_true)));

        for (auto& i: on_true->instructions)
        {
            segment->instructions.push_back(std::move(i));
        }

        segment->instructions.push_back(std::make_unique<irl::Jump>(context.ph_false));
        segment->instructions.push_back(std::make_unique<irl::Label>(std::move(context.ph_false)));

        return segment;
    }

    auto on_false = _on_false->code_gen(context);

    auto ref_end = _var_scope->new_temp(irl::LlvmAtomic::v);

    if (add_jump)
        segment->instructions.push_back(std::make_unique<irl::JumpC>(std::move(condition->out_value), context.ph_true, context.ph_false));

    segment->instructions.push_back(std::make_unique<irl::Label>(std::move(context.ph_true)));

    for (auto& i: on_true->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    segment->instructions.push_back(std::make_unique<irl::Jump>(ref_end));
    segment->instructions.push_back(std::make_unique<irl::Label>(std::move(context.ph_false)));

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

WhileLoop::WhileLoop(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> body):
    _condition(std::move(condition)),
    _body(std::move(body))
{
}

std::string WhileLoop::print()
{
    return "while (" + _condition->print()
        + ")\ndo " + _body->print();
}

std::unique_ptr<irl::IrlSegment> WhileLoop::code_gen(irl::Context context)
{
    auto segment = std::make_unique<irl::IrlSegment>();

    // prepare refs and code generation
    auto ref_condition = _var_scope->new_temp(irl::LlvmAtomic::v);
    auto condition = _condition->code_gen(context);

    auto ref_end = _var_scope->new_placeholder(irl::LlvmAtomic::v);

    irl::Context lcontext;
    lcontext.break_label = ref_end;
    lcontext.continue_label = ref_condition;

    auto ref_body = _var_scope->new_temp(irl::LlvmAtomic::v);
    auto body = _body->code_gen(lcontext);

    _var_scope->fix_placehoder(ref_end);

    // build code segment
    segment->instructions.push_back(std::make_unique<irl::Jump>(ref_condition));
    segment->instructions.push_back(std::make_unique<irl::Label>(ref_condition));

    for (auto& i: condition->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    segment->instructions.push_back(std::make_unique<irl::JumpC>(std::move(condition->out_value), ref_body, ref_end));
    segment->instructions.push_back(std::make_unique<irl::Label>(std::move(ref_body)));

    for (auto& i: body->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    segment->instructions.push_back(std::make_unique<irl::Jump>(std::move(ref_condition)));
    segment->instructions.push_back(std::make_unique<irl::Label>(std::move(ref_end)));

    return segment;
}

void WhileLoop::set_variable_scope(std::shared_ptr<VariableScope> var_scope, std::shared_ptr<FunctionTable> ftable)
{
    _condition->set_variable_scope(var_scope, ftable);
    _body->set_variable_scope(var_scope, ftable);

    _var_scope = std::move(var_scope);
    _ftable = std::move(ftable);
}

std::string Continue::print()
{
    return "continue\n";
}

std::unique_ptr<irl::IrlSegment> Continue::code_gen(irl::Context context)
{
    if (!context.continue_label)
        throw std::logic_error("continue called outside a loop");

    auto segment = std::make_unique<irl::IrlSegment>();

    segment->instructions.push_back(std::make_unique<irl::Jump>(context.continue_label));

    return segment;
}

std::string Break::print()
{
    return "break\n";
}

std::unique_ptr<irl::IrlSegment> Break::code_gen(irl::Context context)
{
    if (!context.break_label)
        throw std::logic_error("break called outside a loop");

    auto segment = std::make_unique<irl::IrlSegment>();

    segment->instructions.push_back(std::make_unique<irl::Jump>(context.break_label));

    return segment;
}
