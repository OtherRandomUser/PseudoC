#pragma once

#include <string>

#include <pseudoc/irl/type.hpp>

namespace irl
{
    struct Value
    {
        virtual ~Value() = default;

        LlvmAtomic tp;

        virtual std::string print() = 0;
    };

    struct Variable : public Value
    {
        std::string id;

        virtual std::string print() override
        {
            return id;
        }
    };

    struct IntLiteral : public Value
    {
        long value;

        std::string print() override
        {
            return std::to_string(value);
        }
    };

    struct Placeholder : public Variable
    {
        Placeholder()
        {
            id = "";
        }

        void fix_id(std::string id)
        {
            if (this->id != "")
                throw std::logic_error("id was already fixed");

            this->id = id;
        }

        std::string print() override
        {
            if (this->id == "")
                throw std::logic_error("id was not fixed");

            return id;
        }
    };
}