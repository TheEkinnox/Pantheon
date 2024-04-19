#pragma once
#include "PantheonCore/ECS/ComponentRegistry.h"

#include <Transform.h>

namespace PantheonCore::ECS
{
    /* Transform */
    template <>
    bool ComponentRegistry::toBinary(const LibMath::Transform&, std::vector<char>&, const EntitiesMap&);

    template <>
    size_t ComponentRegistry::fromBinary(LibMath::Transform&, const char*, size_t, Scene*);

    template <>
    bool ComponentRegistry::toJson(const LibMath::Transform&, rapidjson::Writer<rapidjson::StringBuffer>&, const EntitiesMap&);

    template <>
    bool ComponentRegistry::fromJson(LibMath::Transform&, const rapidjson::Value&, Scene*);

    /* Degree */
    template <>
    bool ComponentRegistry::toBinary(const LibMath::Degree&, std::vector<char>&, const EntitiesMap&);

    template <>
    size_t ComponentRegistry::fromBinary(LibMath::Degree&, const char*, size_t, Scene*);

    template <>
    bool ComponentRegistry::toJson(const LibMath::Degree&, rapidjson::Writer<rapidjson::StringBuffer>&, const EntitiesMap&);

    template <>
    bool ComponentRegistry::fromJson(LibMath::Degree&, const rapidjson::Value&, Scene*);

    /* Radian */
    template <>
    bool ComponentRegistry::toBinary(const LibMath::Radian&, std::vector<char>&, const EntitiesMap&);

    template <>
    size_t ComponentRegistry::fromBinary(LibMath::Radian&, const char*, size_t, Scene*);

    template <>
    bool ComponentRegistry::toJson(const LibMath::Radian&, rapidjson::Writer<rapidjson::StringBuffer>&, const EntitiesMap&);

    template <>
    bool ComponentRegistry::fromJson(LibMath::Radian&, const rapidjson::Value&, Scene*);

    /* ERotationOrder */
    template <>
    bool ComponentRegistry::toBinary(const LibMath::ERotationOrder& component, std::vector<char>& out, const EntitiesMap&);

    template <>
    size_t ComponentRegistry::fromBinary(LibMath::ERotationOrder& out, const char* data, size_t length, Scene*);

    template <>
    bool ComponentRegistry::toJson(
        const LibMath::ERotationOrder& component, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap&);

    template <>
    bool ComponentRegistry::fromJson(LibMath::ERotationOrder& out, const rapidjson::Value& json, Scene*);

    /* Quaternion */
    template <>
    bool ComponentRegistry::toBinary(const LibMath::Quaternion&, std::vector<char>&, const EntitiesMap&);

    template <>
    size_t ComponentRegistry::fromBinary(LibMath::Quaternion&, const char*, size_t, Scene*);

    template <>
    bool ComponentRegistry::toJson(const LibMath::Quaternion&, rapidjson::Writer<rapidjson::StringBuffer>&, const EntitiesMap&);

    template <>
    bool ComponentRegistry::fromJson(LibMath::Quaternion&, const rapidjson::Value&, Scene*);

    /* Vector2 */
    template <>
    bool ComponentRegistry::toBinary(const LibMath::Vector2&, std::vector<char>&, const EntitiesMap&);

    template <>
    size_t ComponentRegistry::fromBinary(LibMath::Vector2&, const char*, size_t, Scene*);

    template <>
    bool ComponentRegistry::toJson(const LibMath::Vector2&, rapidjson::Writer<rapidjson::StringBuffer>&, const EntitiesMap&);

    template <>
    bool ComponentRegistry::fromJson(LibMath::Vector2&, const rapidjson::Value&, Scene*);

    /* Vector3 */
    template <>
    bool ComponentRegistry::toBinary(const LibMath::Vector3&, std::vector<char>&, const EntitiesMap&);

    template <>
    size_t ComponentRegistry::fromBinary(LibMath::Vector3&, const char*, size_t, Scene*);

    template <>
    bool ComponentRegistry::toJson(const LibMath::Vector3&, rapidjson::Writer<rapidjson::StringBuffer>&, const EntitiesMap&);

    template <>
    bool ComponentRegistry::fromJson(LibMath::Vector3&, const rapidjson::Value&, Scene*);

    /* Vector4 */
    template <>
    bool ComponentRegistry::toBinary(const LibMath::Vector4&, std::vector<char>&, const EntitiesMap&);

    template <>
    size_t ComponentRegistry::fromBinary(LibMath::Vector4&, const char*, size_t, Scene*);

    template <>
    bool ComponentRegistry::toJson(const LibMath::Vector4&, rapidjson::Writer<rapidjson::StringBuffer>&, const EntitiesMap&);

    template <>
    bool ComponentRegistry::fromJson(LibMath::Vector4&, const rapidjson::Value&, Scene*);

    /* Matrix3 */
    template <>
    bool ComponentRegistry::toBinary(const LibMath::Matrix3&, std::vector<char>&, const EntitiesMap&);

    template <>
    size_t ComponentRegistry::fromBinary(LibMath::Matrix3&, const char*, size_t, Scene*);

    template <>
    bool ComponentRegistry::toJson(const LibMath::Matrix3&, rapidjson::Writer<rapidjson::StringBuffer>&, const EntitiesMap&);

    template <>
    bool ComponentRegistry::fromJson(LibMath::Matrix3&, const rapidjson::Value&, Scene*);

    /* Matrix4 */
    template <>
    bool ComponentRegistry::toBinary(const LibMath::Matrix4&, std::vector<char>&, const EntitiesMap&);

    template <>
    size_t ComponentRegistry::fromBinary(LibMath::Matrix4&, const char*, size_t, Scene*);

    template <>
    bool ComponentRegistry::toJson(const LibMath::Matrix4&, rapidjson::Writer<rapidjson::StringBuffer>&, const EntitiesMap&);

    template <>
    bool ComponentRegistry::fromJson(LibMath::Matrix4&, const rapidjson::Value&, Scene*);
}
