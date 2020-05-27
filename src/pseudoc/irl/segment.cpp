#include <pseudoc/irl/segment.hpp>

using namespace irl;

std::string IrlSegment::print()
{
    std::string res = "";

    for (auto& instruction: instructions)
    {
        res += instruction->print();
    }

    return res;
}

// void IrlSegment::add_instruction(std::unique_ptr<Instruction> instruction)
// {
//     _instructions.push_back(std::move(instruction));
// }

// void IrlSegment::merge_with(std::unique_ptr<IrlSegment> other)
// {
//     int s = other->_instructions.size();

//     for (int i = 0; i < s; i++)
//     {
//         _instructions.push_back(std::move(other->_instructions[i]));
//     }
// }