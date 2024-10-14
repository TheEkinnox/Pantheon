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

    static_assert(!PantheonCore::Utility::IsAllConst<int>);
    static_assert(PantheonCore::Utility::IsAllConst<const int>);
    static_assert(PantheonCore::Utility::IsAllConst<const int, const float, const char, const bool>);
    static_assert(!PantheonCore::Utility::IsAllConst<int, const float, const char, bool>);

    static_assert(PantheonCore::Utility::IndexOf<int, int> == 0);
    static_assert(PantheonCore::Utility::IndexOf<int, float> == 1);
    static_assert(PantheonCore::Utility::IndexOf<int, int, float> == 0);
    static_assert(PantheonCore::Utility::IndexOf<int, float, char> == 2);
    static_assert(PantheonCore::Utility::IndexOf<int, float, char, int> == 2);

    static_assert(std::is_same_v<PantheonCore::Utility::SmallestInt<1>, int8_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestInt<8>, int8_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestInt<9>, int16_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestInt<16>, int16_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestInt<17>, int32_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestInt<32>, int32_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestInt<33>, int64_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestInt<64>, int64_t>);

    static_assert(std::is_same_v<PantheonCore::Utility::SmallestUInt<1>, uint8_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestUInt<8>, uint8_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestUInt<9>, uint16_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestUInt<16>, uint16_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestUInt<17>, uint32_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestUInt<32>, uint32_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestUInt<33>, uint64_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestUInt<64>, uint64_t>);

    static_assert(std::is_same_v<PantheonCore::Utility::SmallestIntT<int8_t>, int8_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestIntT<uint8_t>, int8_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestIntT<int16_t>, int16_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestIntT<uint16_t>, int16_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestIntT<int32_t>, int32_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestIntT<uint32_t>, int32_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestIntT<float>, int>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestIntT<int64_t>, int64_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestIntT<uint64_t>, int64_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestIntT<double>, long long>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestIntT<long double>, long long>);

    static_assert(std::is_same_v<PantheonCore::Utility::SmallestUIntT<int8_t>, uint8_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestUIntT<uint8_t>, uint8_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestUIntT<int16_t>, uint16_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestUIntT<uint16_t>, uint16_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestUIntT<int32_t>, uint32_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestUIntT<uint32_t>, uint32_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestUIntT<float>, unsigned int>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestUIntT<int64_t>, uint64_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestUIntT<uint64_t>, uint64_t>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestUIntT<double>, unsigned long long>);
    static_assert(std::is_same_v<PantheonCore::Utility::SmallestUIntT<long double>, unsigned long long>);
}
