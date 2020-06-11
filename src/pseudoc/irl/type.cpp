#include <pseudoc/irl/type.hpp>

namespace irl
{
    std::string atomic_to_string(LlvmAtomic a)
    {
        switch (a)
        {
        case v:
            return "void";

        case i8:
            return "i8";

        case i16:
            return "i16";

        case i32:
            return "i32";

        case i64:
            return "i64";

        case fp:
            return "float";

        case db:
            return "double";

        default:
            return "";
        };
    }
}