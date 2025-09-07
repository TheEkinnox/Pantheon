#pragma once
#include "PantheonCore/Resources/ResourceManager.h"
#include "PantheonCore/Resources/ResourceRef.h"
#include "PantheonCore/Resources/ResourceRegistry.h"
#include "PantheonCore/Serialization/IByteSerializable.h"
#include "PantheonCore/Utility/ServiceLocator.h"

namespace PantheonCore::Resources
{
    template <class T>
    ResourceRef<T>::ResourceRef(std::string key, std::string path, T* resource)
        : m_key(std::move(key)), m_path(std::move(path)), m_resource(resource), m_refCount(resource ? new RefCountT(1) : nullptr)
    {
    }

    template <class T>
    ResourceRef<T>::ResourceRef(const std::string& key, const std::string& path)
        : ResourceRef(PTH_SERVICE(ResourceManager).getOrCreate<T>(key, path))
    {
    }

    template <class T>
    ResourceRef<T>::ResourceRef(const ResourceRef& other)
        : m_key(other.m_key), m_path(other.m_path), m_resource(other.m_resource), m_refCount(other.m_refCount)
    {
        if (m_refCount)
            ++(*m_refCount);
    }

    template <class T>
    ResourceRef<T>::ResourceRef(ResourceRef&& other) noexcept
        : m_key(std::move(other.m_key)), m_path(std::move(other.m_path)), m_resource(other.m_resource), m_refCount(other.m_refCount)
    {
        other.m_resource = nullptr;
        other.m_refCount = nullptr;
    }

    template <class T>
    template <typename U>
    ResourceRef<T>::ResourceRef(const ResourceRef<U>& other)
        : m_key(other.m_key), m_path(other.m_path), m_resource(dynamic_cast<T*>(other.m_resource)),
        m_refCount(m_resource ? other.m_refCount : nullptr)
    {
        static_assert(std::is_same_v<T, U> || std::is_base_of_v<T, U> || std::is_base_of_v<U, T>,
            "Attempted to convert to an incompatible resource type");

        PTH_ASSERT((void*)other.m_resource == (void*)m_resource, "Attempted to convert to an incompatible resource type");

        if (m_refCount)
            ++(*m_refCount);
    }

    template <class T>
    template <typename U>
    ResourceRef<T>::ResourceRef(ResourceRef<U>&& other) noexcept
        : m_key(std::move(other.m_key)), m_path(std::move(other.m_path)), m_resource(dynamic_cast<T*>(other.m_resource)),
        m_refCount(m_resource ? other.m_refCount : nullptr)
    {
        static_assert(std::is_same_v<T, U> || std::is_base_of_v<T, U> || std::is_base_of_v<U, T>,
            "Attempted to convert to an incompatible resource type");

        PTH_ASSERT((void*)other.m_resource == (void*)m_resource, "Attempted to convert to an incompatible resource type");

        other.m_resource = nullptr;
        other.m_refCount = nullptr;
    }

    template <class T>
    ResourceRef<T>::~ResourceRef()
    {
        reset();
    }

    template <class T>
    ResourceRef<T>& ResourceRef<T>::operator=(const ResourceRef& other)
    {
        if (this == &other)
            return *this;

        if (other.m_refCount != m_refCount)
        {
            reset();

            if (other.m_refCount)
                ++(*other.m_refCount);
        }

        m_key      = other.m_key;
        m_path     = other.m_path;
        m_resource = other.m_resource;
        m_refCount = other.m_refCount;

        return *this;
    }

    template <class T>
    ResourceRef<T>& ResourceRef<T>::operator=(ResourceRef&& other) noexcept
    {
        if (this == &other)
            return *this;

        if (other.m_refCount != m_refCount)
            reset();

        m_key      = std::move(other.m_key);
        m_path     = std::move(other.m_path);
        m_resource = other.m_resource;
        m_refCount = other.m_refCount;

        other.m_resource = nullptr;
        other.m_refCount = nullptr;

        return *this;
    }

    template <class T>
    bool ResourceRef<T>::operator==(const ResourceRef& other) const
    {
        return m_path == other.m_path;
    }

    template <class T>
    T& ResourceRef<T>::operator*() const
    {
        return *getOrDefault();
    }

    template <class T>
    T* ResourceRef<T>::operator->() const
    {
        return getOrDefault();
    }

    template <class T>
    ResourceRef<T>::operator bool() const
    {
        return m_refCount && getOrDefault();
    }

    template <class T>
    T* ResourceRef<T>::get() const
    {
        return m_resource;
    }

    template <class T>
    T* ResourceRef<T>::getOrDefault() const
    {
        return m_resource || !m_key.empty() || !m_path.empty() ? m_resource : getDefaultResource<T>();
    }

    template <class T>
    typename ResourceRef<T>::RefCountT ResourceRef<T>::getReferenceCount() const
    {
        return m_refCount ? *m_refCount : 0;
    }

    template <class T>
    std::string ResourceRef<T>::getKey() const
    {
        return m_key;
    }

    template <class T>
    std::string ResourceRef<T>::getPath() const
    {
        return m_path;
    }

    template <class T>
    std::string ResourceRef<T>::getFullPath() const
    {
        return PTH_SERVICE(ResourceManager).getFullPath(m_path);
    }

    template <class T>
    bool ResourceRef<T>::reload()
    {
        if constexpr (!std::is_same_v<IResource, T>)
            return (*this) = PTH_SERVICE(ResourceManager).getOrCreate<T>(m_key, m_path);
        else
            return (*this) = PTH_SERVICE(ResourceManager).get<T>(m_key);
    }

    template <class T>
    void ResourceRef<T>::reset()
    {
        if (m_refCount && --(*m_refCount) == 0)
        {
            delete m_refCount;

            if (m_resource)
            {
                delete m_resource;
                m_resource = nullptr;
            }
        }
    }

    template <class T>
    bool ResourceRef<T>::toBinary(std::vector<char>& output) const
    {
        using namespace Serialization;

        const KeySizeT  keySize  = static_cast<KeySizeT>(m_key.size());
        const PathSizeT pathSize = static_cast<PathSizeT>(m_path.size());

        output.reserve(output.size() + sizeof(KeySizeT) + keySize + sizeof(PathSizeT) + pathSize);

        if (!CHECK(IByteSerializable::serializeString<KeySizeT>(m_key, output),
                "Unable to serialize resource ref - Failed to write resource key"))
            return false;

        if (!CHECK(IByteSerializable::serializeString<PathSizeT>(m_path, output),
                "Unable to serialize resource ref - Failed to write resource path"))
            return false;

        return true;
    }

    template <class T>
    size_t ResourceRef<T>::fromBinary(const char* data, const size_t length)
    {
        using namespace Serialization;

        if (!CHECK(data != nullptr && length > 0, "Unable to deserialize resource ref - Invalid buffer"))
            return 0;

        const size_t offset = IByteSerializable::deserializeString<KeySizeT>(m_key, data, length);
        if (!CHECK(offset != 0, "Unable to deserialize resource ref - Key deserialization failed"))
            return 0;

        if (!CHECK(length > offset, "Unable to deserialize resource ref - Invalid offset"))
            return 0;

        const size_t readBytes = IByteSerializable::deserializeString<PathSizeT>(m_path, data + offset, length - offset);
        if (!CHECK(readBytes != 0, "Unable to deserialize resource ref - Path deserialization failed"))
            return 0;

        const std::string basePath = m_path;

        if constexpr (!std::is_same_v<T, IResource>)
            (*this) = { m_key, m_path };

        return CHECK(basePath == m_path, "Unable to deserialize resource ref - Failed to load resource") ? offset + readBytes : 0;
    }

    template <class T>
    bool ResourceRef<T>::toJson(Serialization::JsonWriter& writer) const
    {
        writer.StartObject();

        writer.Key("key");
        writer.String(m_key.c_str(), static_cast<rapidjson::SizeType>(m_key.size()));

        writer.Key("path");
        writer.String(m_path.c_str(), static_cast<rapidjson::SizeType>(m_path.size()));

        return writer.EndObject();
    }

    template <class T>
    bool ResourceRef<T>::fromJson(const Serialization::JsonValue& json)
    {
        if (!CHECK(json.IsObject(), "Unable to deserialize resource ref - Json value should be an object"))
            return false;

        auto it = json.FindMember("key");
        if (!CHECK(it != json.MemberEnd() && it->value.IsString(), "Unable to deserialize resource ref - Invalid resource key"))
            return false;

        m_key = std::string(it->value.GetString(), it->value.GetStringLength());

        it = json.FindMember("path");
        if (!CHECK(it != json.MemberEnd() && it->value.IsString(), "Unable to deserialize resource ref - Invalid resource path"))
            return false;

        m_path = std::string(it->value.GetString(), it->value.GetStringLength());

        const std::string basePath = m_path;

        if constexpr (!std::is_same_v<T, IResource>)
            (*this) = { m_key, m_path };

        return CHECK(basePath == m_path, "Unable to deserialize resource ref - Failed to load resource");
    }

    inline GenericResourceRef::GenericResourceRef(
        std::string type, const std::string& key, const std::string& path, IResource* resource)
        : ResourceRef(key, path, resource), m_type(std::move(type))
    {
        PTH_ASSERT(!m_resource || m_resource->getTypeName() == m_type,
            "Attempted to make generic resource ref with resource of a different type");
    }

    inline GenericResourceRef::GenericResourceRef(const std::string& type, const std::string& key, const std::string& path)
        : GenericResourceRef(PTH_SERVICE(ResourceManager).getOrCreate(type, key, path))
    {
    }

    template <typename T>
    GenericResourceRef::GenericResourceRef(const ResourceRef<T>& other)
        : ResourceRef(other), m_type(m_resource ? m_resource->getTypeName() : std::string())
    {
    }

    template <typename T>
    GenericResourceRef::GenericResourceRef(ResourceRef<T>&& other) noexcept
        : ResourceRef(std::move(other)), m_type(m_resource ? m_resource->getTypeName() : std::string())
    {
    }

    template <typename T>
    GenericResourceRef::GenericResourceRef(const ResourceRef<T>& other, std::string type)
        : ResourceRef(other), m_type(std::move(type))
    {
        PTH_ASSERT(!m_resource || m_resource->getTypeName() == m_type,
            "Attempted to convert resource ref to generic resource ref of a different type");
    }

    template <typename T>
    GenericResourceRef::GenericResourceRef(ResourceRef<T>&& other, std::string type) noexcept
        : ResourceRef(std::move(other)), m_type(std::move(type))
    {
        PTH_ASSERT(!m_resource || m_resource->getTypeName() == m_type,
            "Attempted to convert resource ref to generic resource ref of a different type");
    }

    inline GenericResourceRef::operator bool() const
    {
        return ResourceRef::operator bool() && !m_type.empty();
    }

    inline std::string GenericResourceRef::getType() const
    {
        return m_type;
    }

    inline bool GenericResourceRef::toBinary(std::vector<char>& output) const
    {
        using namespace Serialization;

        if (!CHECK(IByteSerializable::serializeString<TypeSizeT>(m_type, output),
                "Unable to serialize resource ref - Failed to serialize type string"))
            return false;

        return ResourceRef::toBinary(output);
    }

    inline size_t GenericResourceRef::fromBinary(const char* data, const size_t length)
    {
        using namespace Serialization;

        if (!CHECK(data != nullptr && length > 0, "Unable to deserialize resource ref - Invalid buffer"))
            return 0;

        const size_t offset = IByteSerializable::deserializeString<TypeSizeT>(m_type, data, length);
        if (!CHECK(offset != 0, "Unable to deserialize resource ref - Type string deserialization failed"))
            return 0;

        if (!CHECK(length > offset, "Unable to deserialize resource ref - Invalid offset"))
            return 0;

        const size_t readBytes = ResourceRef::fromBinary(data + offset, length - offset);

        if (readBytes == 0)
            return 0;

        const std::string basePath = m_path;

        (*this) = { m_type, m_key, m_path };

        return CHECK(basePath == m_path, "Unable to deserialize resource ref - Failed to load resource") ? offset + readBytes : 0;
    }

    inline bool GenericResourceRef::toJson(Serialization::JsonWriter& writer) const
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

    inline bool GenericResourceRef::fromJson(const Serialization::JsonValue& json)
    {
        const auto it = json.FindMember("type");
        if (!CHECK(it != json.MemberEnd() && it->value.IsString(), "Unable to deserialize resource ref - Invalid resource type"))
            return false;

        m_type = std::string(it->value.GetString(), it->value.GetStringLength());

        if (!ResourceRef::fromJson(json))
            return false;

        const std::string basePath = m_path;

        (*this) = { m_type, m_key, m_path };
        return CHECK(basePath == m_path, "Unable to deserialize resource ref - Failed to load resource");
    }
}
