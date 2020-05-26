#pragma once

#include <string>

namespace irl
{
    enum LlvmAtomic : short
    {
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

    std::string type_to_string(LlvmType t)
    {
        std::string out;

        switch (t.atomic)
        {
        case v:
            out = "void";
            break;

        case i8:
            out = "i8";
            break;

        case i16:
            out = "i16";
            break;

        case i32:
            out = "i32";
            break;

        case i64:
            out = "i64";
            break;

        case fp:
            out = "float";
            break;

        case db:
            out = "double";
            break;

        default:
            out = "";
            break;
        };

        for (auto i = 0; i < t.ptr_level; i++)
        {
            out += "*";
        }

        return out;
    }
}