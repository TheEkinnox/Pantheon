#pragma once
#include "PantheonCore/Serialization/EnumSerializer.h"

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
    bool toJson(const LibMath::Transform&, rapidjson::Writer<rapidjson::StringBuffer>&);
    template <>
    bool fromJson(LibMath::Transform&, const rapidjson::Value&);

    /* Degree */
    template <>
    bool toBinary(const LibMath::Degree&, std::vector<char>&);
    template <>
    size_t fromBinary(LibMath::Degree&, const char*, size_t);

    template <>
    bool toJson(const LibMath::Degree&, rapidjson::Writer<rapidjson::StringBuffer>&);
    template <>
    bool fromJson(LibMath::Degree&, const rapidjson::Value&);

    /* Radian */
    template <>
    bool toBinary(const LibMath::Radian&, std::vector<char>&);
    template <>
    size_t fromBinary(LibMath::Radian&, const char*, size_t);

    template <>
    bool toJson(const LibMath::Radian&, rapidjson::Writer<rapidjson::StringBuffer>&);
    template <>
    bool fromJson(LibMath::Radian&, const rapidjson::Value&);

    /* Quaternion */
    template <>
    bool toBinary(const LibMath::Quaternion&, std::vector<char>&);
    template <>
    size_t fromBinary(LibMath::Quaternion&, const char*, size_t);

    template <>
    bool toJson(const LibMath::Quaternion&, rapidjson::Writer<rapidjson::StringBuffer>&);
    template <>
    bool fromJson(LibMath::Quaternion&, const rapidjson::Value&);

    /* Vector2 */
    template <>
    bool toBinary(const LibMath::Vector2&, std::vector<char>&);
    template <>
    size_t fromBinary(LibMath::Vector2&, const char*, size_t);

    template <>
    bool toJson(const LibMath::Vector2&, rapidjson::Writer<rapidjson::StringBuffer>&);
    template <>
    bool fromJson(LibMath::Vector2&, const rapidjson::Value&);

    /* Vector3 */
    template <>
    bool toBinary(const LibMath::Vector3&, std::vector<char>&);
    template <>
    size_t fromBinary(LibMath::Vector3&, const char*, size_t);

    template <>
    bool toJson(const LibMath::Vector3&, rapidjson::Writer<rapidjson::StringBuffer>&);
    template <>
    bool fromJson(LibMath::Vector3&, const rapidjson::Value&);

    /* Vector4 */
    template <>
    bool toBinary(const LibMath::Vector4&, std::vector<char>&);
    template <>
    size_t fromBinary(LibMath::Vector4&, const char*, size_t);

    template <>
    bool toJson(const LibMath::Vector4&, rapidjson::Writer<rapidjson::StringBuffer>&);
    template <>
    bool fromJson(LibMath::Vector4&, const rapidjson::Value&);

    /* Matrix3 */
    template <>
    bool toBinary(const LibMath::Matrix3&, std::vector<char>&);
    template <>
    size_t fromBinary(LibMath::Matrix3&, const char*, size_t);

    template <>
    bool toJson(const LibMath::Matrix3&, rapidjson::Writer<rapidjson::StringBuffer>&);
    template <>
    bool fromJson(LibMath::Matrix3&, const rapidjson::Value&);

    /* Matrix4 */
    template <>
    bool toBinary(const LibMath::Matrix4&, std::vector<char>&);
    template <>
    size_t fromBinary(LibMath::Matrix4&, const char*, size_t);

    template <>
    bool toJson(const LibMath::Matrix4&, rapidjson::Writer<rapidjson::StringBuffer>&);
    template <>
    bool fromJson(LibMath::Matrix4&, const rapidjson::Value&);
}
