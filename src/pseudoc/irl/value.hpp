#pragma once

#include <string>

#include <pseudoc/irl/type.hpp>

namespace irl
{
    struct Value
    {
        LlvmType tp;

        virtual std::string print() = 0;
    };

    struct Variable : public Value
    {
        std::string id;

        std::string print() override
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
}