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

        std::string print();
    };
}