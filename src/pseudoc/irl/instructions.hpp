#pragma once

#include <memory>
#include <string>

#include <pseudoc/irl/type.hpp>
#include <pseudoc/irl/value.hpp>

namespace irl
{
    class Instruction
    {
    public:
        virtual ~Instruction() = default;

        virtual std::string print() = 0;
    };

    class Alloca : public Instruction
    {
    public:
        Alloca(std::shared_ptr<Variable> out_var, short alignment);

        std::string print() override;

    private:
        std::shared_ptr<Value> _out_var;
        short _alignment;
    };

    class Store : public Instruction
    {
    public:
        Store(std::shared_ptr<Value> from, std::shared_ptr<Value> to, short alignment);

        std::string print() override;

    private:
        std::shared_ptr<Value> _from;
        std::shared_ptr<Value> _to;
        short _alignment;
    };

    class Load : public Instruction
    {
    public:
        Load(std::shared_ptr<Variable> from, std::shared_ptr<Variable> to, short alignment);

        std::string print() override;

    private:
        std::shared_ptr<Value> _from;
        std::shared_ptr<Value> _to;
        short _alignment;
    };

    class Add : public Instruction
    {
    public:
        Add(std::shared_ptr<Variable> out, std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs, LlvmAtomic tp);

        std::string print() override;

    private:
        std::shared_ptr<Variable> _out;
        std::shared_ptr<Value> _lhs;
        std::shared_ptr<Value> _rhs;
        LlvmAtomic _tp;
    };

    class Sub : public Instruction
    {
    public:
        Sub(std::shared_ptr<Variable> out, std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs, LlvmAtomic tp);

        std::string print() override;

    private:
        std::shared_ptr<Variable> _out;
        std::shared_ptr<Value> _lhs;
        std::shared_ptr<Value> _rhs;
        LlvmAtomic _tp;
    };

    class Mul : public Instruction
    {
    public:
        Mul(std::shared_ptr<Variable> out, std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs, LlvmAtomic tp);

        std::string print() override;

    private:
        std::shared_ptr<Variable> _out;
        std::shared_ptr<Value> _lhs;
        std::shared_ptr<Value> _rhs;
        LlvmAtomic _tp;
    };

    class SDiv : public Instruction
    {
    public:
        SDiv(std::shared_ptr<Variable> out, std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs, LlvmAtomic tp);

        std::string print() override;

    private:
        std::shared_ptr<Variable> _out;
        std::shared_ptr<Value> _lhs;
        std::shared_ptr<Value> _rhs;
        LlvmAtomic _tp;
    };

    class Def : public Instruction
    {
    public:
        Def(std::string id, const FunctionDef& def);

        std::string print() override;

    private:
        std::string _id;
        FunctionDef _def;
    };

    class EndDef : public Instruction
    {
    public:
        std::string print() override;
    };

    class Ret : public Instruction
    {
    public:
        Ret(std::shared_ptr<Value> res, LlvmAtomic tp);

        std::string print() override;

    private:
        std::shared_ptr<Value> _res;
        LlvmAtomic _tp;
    };

    class Label : public Instruction
    {
    public:
        Label(std::shared_ptr<Variable> ref);

        std::string print() override;

        std::shared_ptr<Variable> get_ref();

    private:
        std::shared_ptr<Variable> _ref;
    };

    class Jump : public Instruction
    {
    public:
        Jump(std::shared_ptr<Variable> label_ref);

        std::string print() override;

    private:
        std::shared_ptr<Variable> _label_ref;
    };

    class JumpC : public Instruction
    {
    public:
        JumpC(std::shared_ptr<Value> condition, std::shared_ptr<Variable> on_true, std::shared_ptr<Variable> on_false);

        std::string print() override;

    private:
        std::shared_ptr<Value> _condition;
        std::shared_ptr<Variable> _on_true;
        std::shared_ptr<Variable> _on_false;
    };
}