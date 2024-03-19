#pragma once
#include <PantheonCore/Utility/TypeTraits.h>

namespace PantheonTest
{
    static_assert(PantheonCore::Utility::IsOneOf<int, int>);
    static_assert(!PantheonCore::Utility::IsOneOf<int, float>);
    static_assert(PantheonCore::Utility::IsOneOf<int, float, char, int>);
    static_assert(!PantheonCore::Utility::IsOneOf<int, float, char, bool>);

    static_assert(!PantheonCore::Utility::HasDuplicates<int>);
    static_assert(PantheonCore::Utility::HasDuplicates<int, int>);
    static_assert(PantheonCore::Utility::HasDuplicates<int, float, char, int>);
    static_assert(!PantheonCore::Utility::HasDuplicates<int, float, char, bool>);

    static_assert(PantheonCore::Utility::IndexOf<int, int> == 0);
    static_assert(PantheonCore::Utility::IndexOf<int, float> == 1);
    static_assert(PantheonCore::Utility::IndexOf<int, int, float> == 0);
    static_assert(PantheonCore::Utility::IndexOf<int, float, char> == 2);
    static_assert(PantheonCore::Utility::IndexOf<int, float, char, int> == 2);
}
