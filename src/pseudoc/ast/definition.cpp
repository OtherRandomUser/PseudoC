#include <pseudoc/ast/definition.hpp>

using namespace ast;

FunctionParam::FunctionParam(std::string identifier, irl::LlvmAtomic tp):
    _identifier(std::move(identifier))
{
    _tp = tp;
}

std::string FunctionParam::print()
{
    return irl::atomic_to_string(_tp) + " " + _identifier;
}

std::unique_ptr<irl::IrlSegment> FunctionParam::code_gen(irl::Context context)
{
    auto segment = std::make_unique<irl::IrlSegment>();

    auto ref = _var_scope->add_variable(_identifier, _tp);

    segment->instructions.push_back(std::make_unique<irl::Alloca>(ref, 4));
    segment->instructions.push_back(std::make_unique<irl::Store>(_param_ref, ref, 4));

    segment->out_value = std::move(ref);

    return segment;
}

void FunctionParam::add_temp()
{
    _param_ref = _var_scope->new_temp(_tp);
}

FunctionDefinition::FunctionDefinition(std::string identifier, irl::LlvmAtomic tp, std::unique_ptr<std::vector<std::unique_ptr<FunctionParam>>> params, std::unique_ptr<CompoundStatement> body):
    _identifier(std::move(identifier)),
    _params(std::move(*params)),
    _body(std::move(body))
{
    _tp = tp;
}

std::string FunctionDefinition::print()
{
    std::string params = "(";
    std::string junc = "";

    for (auto& p: _params)
    {
        params += junc + p->print();
        junc = ", ";
    }

    params += ")\n";

    return irl::atomic_to_string(_tp)
        + " " + _identifier + params
        + _body->print() + "\n";
}

std::unique_ptr<irl::IrlSegment> FunctionDefinition::code_gen(irl::Context context)
{
    auto segment = std::make_unique<irl::IrlSegment>();

    irl::FunctionDef def;
    def.tp = _tp;

    for(auto& p: _params)
    {
        p->add_temp();
        def.params.push_back(p->get_type());
    }

    _ftable->add_function(_identifier, def);

    segment->instructions.push_back(std::make_unique<irl::Def>(_identifier, def));

    _var_scope->skip();

    for(auto& p: _params)
    {
        auto s = p->code_gen(context);

        for (auto& i: s->instructions)
        {
            segment->instructions.push_back(std::move(i));
        }
    }

    irl::Context fcontext;
    fcontext.continue_label = nullptr;
    fcontext.break_label = nullptr;

    auto body = _body->code_gen(std::move(fcontext));

    for (auto& i: body->instructions)
    {
        segment->instructions.push_back(std::move(i));
    }

    segment->instructions.push_back(std::make_unique<irl::EndDef>());
    return segment;
}
