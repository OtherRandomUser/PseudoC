#pragma once

#include <string>

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
        db
    };

    std::string atomic_to_string(LlvmAtomic a);
}