#pragma once
#include "PantheonCore/Serialization/Serializer.h"

#include <Transform.h>
#include <Vector.h>

namespace PantheonCore::Serialization
{
    /* Transform */
    template <>
    bool toBinary(const LibMath::Transform&, std::vector<char>&);
    template <>
    size_t fromBinary(LibMath::Transform&, const char*, size_t);

    template <>
    bool toJson(const LibMath::Transform&, JsonWriter&);
    template <>
    bool fromJson(LibMath::Transform&, const JsonValue&);

    /* Degree */
    template <>
    bool toBinary(const LibMath::Degree&, std::vector<char>&);
    template <>
    size_t fromBinary(LibMath::Degree&, const char*, size_t);

    template <>
    bool toJson(const LibMath::Degree&, JsonWriter&);
    template <>
    bool fromJson(LibMath::Degree&, const JsonValue&);

    /* Radian */
    template <>
    bool toBinary(const LibMath::Radian&, std::vector<char>&);
    template <>
    size_t fromBinary(LibMath::Radian&, const char*, size_t);

    template <>
    bool toJson(const LibMath::Radian&, JsonWriter&);
    template <>
    bool fromJson(LibMath::Radian&, const JsonValue&);

    /* Quaternion */
    template <>
    bool toBinary(const LibMath::Quaternion&, std::vector<char>&);
    template <>
    size_t fromBinary(LibMath::Quaternion&, const char*, size_t);

    template <>
    bool toJson(const LibMath::Quaternion&, JsonWriter&);
    template <>
    bool fromJson(LibMath::Quaternion&, const JsonValue&);

    /* Vector2 */
    template <>
    bool toBinary(const LibMath::Vector2&, std::vector<char>&);
    template <>
    size_t fromBinary(LibMath::Vector2&, const char*, size_t);

    template <>
    bool toJson(const LibMath::Vector2&, JsonWriter&);
    template <>
    bool fromJson(LibMath::Vector2&, const JsonValue&);

    /* Vector3 */
    template <>
    bool toBinary(const LibMath::Vector3&, std::vector<char>&);
    template <>
    size_t fromBinary(LibMath::Vector3&, const char*, size_t);

    template <>
    bool toJson(const LibMath::Vector3&, JsonWriter&);
    template <>
    bool fromJson(LibMath::Vector3&, const JsonValue&);

    /* Vector4 */
    template <>
    bool toBinary(const LibMath::Vector4&, std::vector<char>&);
    template <>
    size_t fromBinary(LibMath::Vector4&, const char*, size_t);

    template <>
    bool toJson(const LibMath::Vector4&, JsonWriter&);
    template <>
    bool fromJson(LibMath::Vector4&, const JsonValue&);

    /* Matrix3 */
    template <>
    bool toBinary(const LibMath::Matrix3&, std::vector<char>&);
    template <>
    size_t fromBinary(LibMath::Matrix3&, const char*, size_t);

    template <>
    bool toJson(const LibMath::Matrix3&, JsonWriter&);
    template <>
    bool fromJson(LibMath::Matrix3&, const JsonValue&);

    /* Matrix4 */
    template <>
    bool toBinary(const LibMath::Matrix4&, std::vector<char>&);
    template <>
    size_t fromBinary(LibMath::Matrix4&, const char*, size_t);

    template <>
    bool toJson(const LibMath::Matrix4&, JsonWriter&);
    template <>
    bool fromJson(LibMath::Matrix4&, const JsonValue&);
}
