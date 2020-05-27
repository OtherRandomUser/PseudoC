#include <pseudoc/irl/instructions.hpp>

using namespace irl;

Alloca::Alloca(std::shared_ptr<Variable> out_var, short alignment):
    _out_var(std::move(out_var)),
    _alignment(alignment)
{
}

std::string Alloca::print()
{
    auto t = _out_var->tp;
    t.ptr_level -= 1;

    return _out_var->print()
        + " = alloca "
        + type_to_string(t)
        + ", align "
        + std::to_string(_alignment)
        + "\n";
}

Store::Store(std::shared_ptr<Value> from, std::shared_ptr<Variable> to, short alignment):
    _from(std::move(from)),
    _to(std::move(to)),
    _alignment(alignment)
{
}

std::string Store::print()
{
    std::string res = "store ";
    return res
        + type_to_string(_from->tp)
        + " " + _from->print()
        + ", " + type_to_string(_from->tp)
        + " " + _to->print()
        + ", align " + std::to_string(_alignment)
        + "\n";
}