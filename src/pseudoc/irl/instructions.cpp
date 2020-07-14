#include <pseudoc/irl/instructions.hpp>

using namespace irl;

Alloca::Alloca(std::shared_ptr<Variable> out_var, short alignment):
    _out_var(std::move(out_var)),
    _alignment(alignment)
{
}

std::string Alloca::print()
{
    return "  " + _out_var->print()
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
    std::string res = "  store ";
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
    return "  " + _to->print()
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
    return "  " + _out->print()
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
    return "  " + _out->print()
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
    return "  " + _out->print()
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
    return "  " + _out->print()
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

Ret::Ret(std::shared_ptr<Value> res, LlvmAtomic tp):
    _res(std::move(res))
{
    _tp = tp;
}

std::string Ret::print()
{
    return "  ret " + atomic_to_string(_tp) + " " + _res->print() + "\n";
}

Label::Label(std::shared_ptr<Variable> ref):
    _ref(std::move(ref))
{
}

std::string Label::print()
{
    return "\n; <label>:" + _ref->print() + ":\n";
}

std::shared_ptr<Variable> Label::get_ref()
{
    return _ref;
}

Jump::Jump(std::shared_ptr<Variable> label_ref):
    _label_ref(std::move(label_ref))
{
}

std::string Jump::print()
{
    return "  br label " + _label_ref->print() + "\n";
}

JumpC::JumpC(std::shared_ptr<Value> condition, std::shared_ptr<Variable> on_true, std::shared_ptr<Variable> on_false):
    _condition(std::move(condition)),
    _on_true(std::move(on_true)),
    _on_false(std::move(on_false))
{
}

std::string JumpC::print()
{
    return "  br i1 " + _condition->print() + ", label " + _on_true->print() + ", label " + _on_false->print() + "\n";
}

ICmp::ICmp(ICmp::CondT cond, std::shared_ptr<Variable> out, std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs, LlvmAtomic tp):
    _cond(std::move(cond)),
    _out(std::move(out)),
    _lhs(std::move(lhs))
{
    _rhs = rhs;
    _tp = tp;
}

std::string ICmp::print()
{
    std::string t;

    switch (_cond)
    {
    case CondT::eq:
        t = "eq";
        break;

    case CondT::ne:
        t = "ne";
        break;

    case CondT::ugt:
        t = "ugt";
        break;

    case CondT::uge:
        t = "uge";
        break;

    case CondT::ult:
        t = "ult";
        break;

    case CondT::ule:
        t = "ule";
        break;

    case CondT::sgt:
        t = "sgt";
        break;

    case CondT::sge:
        t = "sge";
        break;

    case CondT::slt:
        t = "slt";
        break;

    case CondT::sle:
        t = "sle";
        break;
    }

    return "  " + _out->print() + " = icmp " + t + " "
        + atomic_to_string(_tp) + " "
        + _lhs->print() + ", "
        + _rhs->print() + "\n";
}

Phi::Phi(std::shared_ptr<Variable> out, LlvmAtomic tp):
    _out(std::move(out))
{
    _tp = tp;
}

void Phi::add_branch(std::shared_ptr<Value> val, std::shared_ptr<Variable> origin)
{
    if (val->tp != _tp)
        throw std::logic_error("Incompatible types");

    _branches.push_back({
        .val = std::move(val),
        .origin = std::move(origin)
    });
}

std::string Phi::print()
{
    if (_branches.size() < 2)
        throw std::logic_error("Phi command used with less than two operrands");

    std::string res = _out->print() + " = phi " + atomic_to_string(_tp);
    std::string junc = "";

    for (const auto& branch: _branches)
    {
        res += junc + " [ " + branch.val->print() + ", " + branch.origin->print() + " ]";
        junc = ",";
    }

    res += '\n';
    return res;
}

ZExt::ZExt(std::shared_ptr<Value> in, LlvmAtomic tp1, std::shared_ptr<Variable> out, LlvmAtomic tp2):
    _in(std::move(in)),
    _out(std::move(out))
{
}

std::string ZExt::print()
{
    // %31 = zext i1 %30 to i32
    return _out->print() + " = "
        + atomic_to_string(_in->tp) + " "
        + _in->print() + " to "
        + atomic_to_string(_out->tp) + "\n";
}

Call::Call(std::string id, std::shared_ptr<Variable> out, LlvmAtomic tp):
    _id(id),
    _out(std::move(out))
{
    _tp = tp;
}

void Call::add_param(std::shared_ptr<Value> param)
{
    _params.push_back(std::move(param));
}

std::string Call::print()
{
    // %5 = call i32 @func(i32 1, i32 %4, i32 3)
    std::string l = _tp == LlvmAtomic::v ? "  " : "  " + _out->print() + " = ";
    l += "call " + atomic_to_string(_tp) + " @" + _id + "(";
    std::string junc = "";

    for (auto& param: _params)
    {
        l += junc + atomic_to_string(param->tp) + " " + param->print();
        junc = ", ";
    }

    return l + ")\n";
}
