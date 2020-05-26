#pragma once

#include <string>

namespace irl
{
    enum IrlInstruction
    {
        Assign = 0,
        Add = 1,
        Subtract = 2,
        Multiply = 3,
        Divide = 4
    };

    class IrlSegment
    {
    public:
        std::string print()
        {
            return "";
        }
    };
}