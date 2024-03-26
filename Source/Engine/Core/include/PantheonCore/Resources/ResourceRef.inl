#pragma once
#include "PantheonCore/Resources/ResourceManager.h"
#include "PantheonCore/Resources/ResourceRef.h"
#include "PantheonCore/Utility/ServiceLocator.h"

namespace PantheonCore::Resources
{
    inline ResourceRefBase::ResourceRefBase(std::string key, std::string path)
        : m_key(std::move(key)), m_path(std::move(path))
    {
    }

    inline ResourceRefBase::ResourceRefBase(const ResourceRefBase& other)
        : ResourceRefBase(other.m_key, other.m_path)
    {
    }

    inline ResourceRefBase::ResourceRefBase(ResourceRefBase&& other) noexcept
    {
        m_key.swap(other.m_key);
        m_path.swap(other.m_path);
    }

    inline ResourceRefBase& ResourceRefBase::operator=(const ResourceRefBase& other)
    {
        if (this == &other)
            return *this;

        m_key  = other.m_key;
        m_path = other.m_path;

        return *this;
    }

    inline ResourceRefBase& ResourceRefBase::operator=(ResourceRefBase&& other) noexcept
    {
        if (this == &other)
            return *this;

        m_key.swap(other.m_key);
        m_path.swap(other.m_path);

        return *this;
    }

    inline std::string ResourceRefBase::getKey() const
    {
        return m_key;
    }

    inline std::string ResourceRefBase::getPath() const
    {
        return m_path;
    }

    inline bool ResourceRefBase::hasValue() const
    {
        return !m_key.empty() && !m_path.empty();
    }

    inline bool ResourceRefBase::toJson(rapidjson::Writer<rapidjson::StringBuffer>& writer) const
    {
        writer.StartObject();

        writer.Key("key");
        writer.String(m_key.c_str(), static_cast<rapidjson::SizeType>(m_key.size()));

        writer.Key("path");
        writer.String(m_path.c_str(), static_cast<rapidjson::SizeType>(m_path.size()));

        return writer.EndObject();
    }

    inline bool ResourceRefBase::fromJson(const rapidjson::Value& json)
    {
        if (!json.IsObject())
        {
            DEBUG_LOG_ERROR("Unable to deserialize resource ref - Json value should be an object");
            return false;
        }

        auto it = json.FindMember("key");
        if (it == json.MemberEnd() || !it->value.IsString())
        {
            DEBUG_LOG_ERROR("Unable to deserialize resource ref - Invalid resource key");
            return false;
        }

        m_key = it->value.GetString();

        it = json.FindMember("path");
        if (it == json.MemberEnd() || !it->value.IsString())
        {
            DEBUG_LOG_ERROR("Unable to deserialize resource ref - Invalid resource path");
            return false;
        }

        m_path = it->value.GetString();
        return true;
    }

    inline bool ResourceRefBase::toBinary(std::vector<char>& output) const
    {
        const KeySizeT  keySize  = static_cast<KeySizeT>(m_key.size());
        const PathSizeT pathSize = static_cast<PathSizeT>(m_path.size());

        output.reserve(output.size() + sizeof(KeySizeT) + keySize + sizeof(PathSizeT) + pathSize);

        if (!serializeString<KeySizeT>(m_key, output))
        {
            DEBUG_LOG_ERROR("Unable to serialize resource ref - Failed to write resource key");
            return false;
        }

        if (!serializeString<PathSizeT>(m_path, output))
        {
            DEBUG_LOG_ERROR("Unable to serialize resource ref - Failed to write resource path");
            return false;
        }

        return true;
    }

    inline size_t ResourceRefBase::fromBinary(const char* data, const size_t length)
    {
        if (data == nullptr || length == 0)
        {
            DEBUG_LOG_ERROR("Unable to deserialize resource ref - Invalid buffer");
            return 0;
        }

        if (!CHECK(deserializeString<KeySizeT>(m_key, data, length) != 0,
                "Unable to deserialize resource ref - Key deserialization failed"))
            return 0;

        const size_t offset = sizeof(KeySizeT) + m_key.size();
        if (!CHECK(length > offset, "Unable to deserialize resource ref - Invalid offset"))
            return 0;

        if (!CHECK(deserializeString<PathSizeT>(m_path, data + offset, length - offset) != 0,
                "Unable to deserialize resource ref - Path deserialization failed"))
            return 0;

        return offset + sizeof(PathSizeT) + m_path.size();
    }

    template <class T>
    ResourceRef<T>::ResourceRef(const std::string& key, const std::string& path)
        : ResourceRefBase(key, path)
    {
    }

    template <class T>
    T* ResourceRef<T>::operator*() const
    {
        if (!hasValue())
            return getDefaultResource<T>();

        return PTH_SERVICE(ResourceManager).getOrCreate<T>(m_key, m_path);
    }

    template <class T>
    T* ResourceRef<T>::operator->() const
    {
        return **this;
    }

    inline GenericResourceRef::GenericResourceRef(std::string type, const std::string& key, const std::string& path)
        : ResourceRefBase(key, path), m_type(std::move(type))
    {
    }

    inline IResource* GenericResourceRef::operator*() const
    {
        if (!hasValue())
            return IResource::getDefault(m_type);

        return PTH_SERVICE(ResourceManager).getOrCreate(m_type, m_key, m_path);
    }

    inline IResource* GenericResourceRef::operator->() const
    {
        return **this;
    }

    inline bool GenericResourceRef::hasValue() const
    {
        return ResourceRefBase::hasValue() && !m_type.empty();
    }

    inline bool GenericResourceRef::toJson(rapidjson::Writer<rapidjson::StringBuffer>& writer) const
    {
        writer.StartObject();

        writer.Key("type");
        writer.String(m_type.c_str(), static_cast<rapidjson::SizeType>(m_type.size()));

        writer.Key("key");
        writer.String(m_key.c_str(), static_cast<rapidjson::SizeType>(m_key.size()));

        writer.Key("path");
        writer.String(m_path.c_str(), static_cast<rapidjson::SizeType>(m_path.size()));

        return writer.EndObject();
    }

    inline bool GenericResourceRef::fromJson(const rapidjson::Value& json)
    {
        if (!ResourceRefBase::fromJson(json))
            return false;

        const auto it = json.FindMember("type");
        if (it == json.MemberEnd() || !it->value.IsString())
        {
            DEBUG_LOG_ERROR("Unable to deserialize resource ref - Invalid resource type");
            return false;
        }

        m_type = it->value.GetString();

        return true;
    }

    inline bool GenericResourceRef::toBinary(std::vector<char>& output) const
    {
        if (!serializeString<TypeSizeT>(m_type, output))
        {
            DEBUG_LOG_ERROR("Unable to serialize resource ref - Failed to serialize type string");
            return false;
        }

        return ResourceRefBase::toBinary(output);
    }

    inline size_t GenericResourceRef::fromBinary(const char* data, size_t length)
    {
        if (data == nullptr || length == 0)
        {
            DEBUG_LOG_ERROR("Unable to deserialize resource ref - Invalid buffer");
            return 0;
        }

        if (!CHECK(deserializeString<TypeSizeT>(m_type, data, length) != 0,
                "Unable to deserialize resource ref - Key deserialization failed"))
            return 0;

        const size_t offset = sizeof(TypeSizeT) + m_type.size();

        if (length <= offset)
        {
            DEBUG_LOG_ERROR("Unable to deserialize resource ref - Invalid offset");
            return 0;
        }

        return ResourceRefBase::fromBinary(data + offset, length - offset);
    }
}
