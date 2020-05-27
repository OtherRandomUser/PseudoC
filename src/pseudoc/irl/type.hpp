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

    struct LlvmType
    {
        LlvmAtomic atomic;
        short ptr_level = 0;
    };

    std::string type_to_string(LlvmType t);
}