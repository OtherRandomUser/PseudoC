#include <pseudoc/irl/instructions.hpp>

using namespace irl;

Alloca::Alloca(std::shared_ptr<Variable> out_var, short alignment):
    _out_var(std::move(out_var)),
    _alignment(alignment)
{
}

std::string Alloca::print()
{
    return _out_var->print()
        + " = alloca "
        + atomic_to_string(_out_var->tp)
        + ", align "
        + std::to_string(_alignment)
        + "\n";
}

Store::Store(std::shared_ptr<Value> from, std::shared_ptr<Value> to, short alignment):
    _from(std::move(from)),
    _to(std::move(to)),
    _alignment(alignment)
{
}

std::string Store::print()
{
    std::string res = "store ";
    return res
        + atomic_to_string(_from->tp)
        + " " + _from->print()
        + ", " + atomic_to_string(_from->tp)
        + "* " + _to->print()
        + ", align " + std::to_string(_alignment)
        + "\n";
}

Load::Load(std::shared_ptr<Variable> from, std::shared_ptr<Variable> to, short alignment):
    _from(std::move(from)),
    _to(std::move(to)),
    _alignment(alignment)
{
}

std::string Load::print()
{
    return _to->print()
        + " = load "
        + atomic_to_string(_to->tp)
        + ", "
        + atomic_to_string(_from->tp)
        + "* " + _from->print()
        + ", align "
        + std::to_string(_alignment)
        + "\n";
}

Add::Add(std::shared_ptr<Variable> out, std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs, LlvmAtomic tp):
    _out(std::move(out)),
    _lhs(std::move(lhs)),
    _rhs(std::move(rhs))
{
    _tp = tp;

    if (_out->tp != tp || _lhs->tp != tp || _rhs->tp != tp)
        throw std::logic_error("Type mismatch");
}

std::string Add::print()
{
    return _out->print()
        + " = add nsw "
        + atomic_to_string(_tp)
        + " " + _lhs->print()
        + ", " + _rhs->print()
        + "\n";
}

Sub::Sub(std::shared_ptr<Variable> out, std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs, LlvmAtomic tp):
    _out(std::move(out)),
    _lhs(std::move(lhs)),
    _rhs(std::move(rhs))
{
    _tp = tp;

    if (_out->tp != tp || _lhs->tp != tp || _rhs->tp != tp)
        throw std::logic_error("Type mismatch");
}

std::string Sub::print()
{
    return _out->print()
        + " = sub nsw "
        + atomic_to_string(_tp)
        + " " + _lhs->print()
        + ", " + _rhs->print()
        + "\n";
}

Mul::Mul(std::shared_ptr<Variable> out, std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs, LlvmAtomic tp):
    _out(std::move(out)),
    _lhs(std::move(lhs)),
    _rhs(std::move(rhs))
{
    _tp = tp;

    if (_out->tp != tp || _lhs->tp != tp || _rhs->tp != tp)
        throw std::logic_error("Type mismatch");
}

std::string Mul::print()
{
    return _out->print()
        + " = mul nsw "
        + atomic_to_string(_tp)
        + " " + _lhs->print()
        + ", " + _rhs->print()
        + "\n";
}

SDiv::SDiv(std::shared_ptr<Variable> out, std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs, LlvmAtomic tp):
    _out(std::move(out)),
    _lhs(std::move(lhs)),
    _rhs(std::move(rhs))
{
    _tp = tp;

    if (_out->tp != tp || _lhs->tp != tp || _rhs->tp != tp)
        throw std::logic_error("Type mismatch");
}

std::string SDiv::print()
{
    return _out->print()
        + " = sdiv nsw "
        + atomic_to_string(_tp)
        + " " + _lhs->print()
        + ", " + _rhs->print()
        + "\n";
}

Def::Def(std::string id, const FunctionDef& def):
    _id(std::move(id)),
    _def(def)
{
}

std::string Def::print()
{
    std::string params = "(";
    std::string junc = "";

    for (auto& tp: _def.params)
    {
        params += junc + atomic_to_string(tp);
        junc = ", ";
    }

    params += ")";

    return "define " + atomic_to_string(_def.tp) + " @" + _id + params + " #0 {\n";
}

std::string EndDef::print()
{
    return "}\n";
}
