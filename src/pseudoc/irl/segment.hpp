#pragma once

#include <string>
#include <vector>

#include <pseudoc/irl/instructions.hpp>
#include <pseudoc/irl/value.hpp>

namespace irl
{
    struct IrlSegment
    {
        std::vector<std::unique_ptr<Instruction>> instructions;
        std::shared_ptr<Value> out_value;
        std::shared_ptr<Value> out_false;

        std::string print();
    };

    struct Context
    {
        std::shared_ptr<Variable> continue_label;
        std::shared_ptr<Variable> break_label;

        std::shared_ptr<Placeholder> ph_true = nullptr;
        std::shared_ptr<Placeholder> ph_false = nullptr;
    };
}