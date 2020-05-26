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
        Alloca(std::unique_ptr<Variable> out_var, short alignment);

        std::string print() override;

    private:
        std::unique_ptr<Value> _out_var;
        short _alignment;
    };

    class Store : public Instruction
    {
    public:
        Store(std::unique_ptr<Value> from, std::unique_ptr<Variable> to, short alignment);

        std::string print() override;

    private:
        std::unique_ptr<Value> _from;
        std::unique_ptr<Value> _to;
        short _alignment;
    };
}