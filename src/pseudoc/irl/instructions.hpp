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
}