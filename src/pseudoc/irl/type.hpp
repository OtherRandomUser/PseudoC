#pragma once

#include <string>
#include <vector>

namespace irl
{
    enum LlvmAtomic : short
    {
        error,
        v,
        i8,
        i16,
        i32,
        i64,
        fp,
        db,
        b
    };

    std::string atomic_to_string(LlvmAtomic a);

    struct FunctionDef
    {
        LlvmAtomic tp;
        std::vector<LlvmAtomic> params;
    };
}