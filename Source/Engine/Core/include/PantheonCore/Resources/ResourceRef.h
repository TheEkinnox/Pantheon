#pragma once
#include "PantheonCore/Resources/IResource.h"
#include "PantheonCore/Serialization/IByteSerializable.h"
#include "PantheonCore/Serialization/IJsonSerializable.h"

#include <type_traits>

namespace PantheonCore::Resources
{
    class ResourceRefBase : public Serialization::IJsonSerializable, public Serialization::IByteSerializable
    {
    public:
        using KeySizeT = uint8_t;
        using PathSizeT = uint8_t;

        ResourceRefBase() = default;
        ResourceRefBase(std::string key, std::string path);
        ResourceRefBase(const ResourceRefBase& other);
        ResourceRefBase(ResourceRefBase&& other) noexcept;

        ResourceRefBase& operator=(const ResourceRefBase& other);
        ResourceRefBase& operator=(ResourceRefBase&& other) noexcept;

        ~ResourceRefBase() override = default;

        /**
         * \brief Gets the referenced resource's key
         * \return The referenced resource's key
         */
        std::string getKey() const;

        /**
         * \brief Gets the referenced resource's path
         * \return The referenced resource's path
         */
        std::string getPath() const;

        /**
         * \brief Checks whether the resource reference has been set or not.
         * \return True if the resource ref is set. False otherwise.
         */
        virtual bool hasValue() const;

        /**
         * \brief Serializes the resource reference to json
         * \param writer The output json writer
         * \return True on success. False otherwise.
         */
        bool serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override;

        /**
         * \brief Deserializes the resource reference from json
         * \param json The input json data
         * \return True on success. False otherwise.
         */
        bool deserialize(const rapidjson::Value& json) override;

        /**
         * \brief Serializes the resource reference to a byte array
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        bool serialize(std::vector<char>& output) const override;

        /**
         * \brief Deserializes the resource reference from the given memory buffer
         * \note On success the read bytes will always be sizeof(KeySizeT) + getKey().size() + sizeof(PathSizeT) + getPath().size()
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return The number of deserialized bytes on success. 0 otherwise.
         */
        size_t deserialize(const void* data, size_t length) override;

    protected:
        std::string m_key;
        std::string m_path;
    };

    template <class T>
    class ResourceRef final : public ResourceRefBase
    {
        static_assert(std::is_base_of_v<IResource, T>);

    public:
        ResourceRef() = default;
        ResourceRef(const std::string& key, const std::string& path);
        ResourceRef(const ResourceRef& other)     = default;
        ResourceRef(ResourceRef&& other) noexcept = default;

        ResourceRef& operator=(const ResourceRef& other)     = default;
        ResourceRef& operator=(ResourceRef&& other) noexcept = default;

        ~ResourceRef() override = default;

        operator T*() const;
        T* operator*() const;
        T* operator->() const;
    };

    class GenericResourceRef final : public ResourceRefBase
    {
    public:
        using TypeSizeT = uint8_t;

        GenericResourceRef() = default;
        GenericResourceRef(std::string type, const std::string& key, const std::string& path);
        GenericResourceRef(const GenericResourceRef& other)     = default;
        GenericResourceRef(GenericResourceRef&& other) noexcept = default;

        GenericResourceRef& operator=(const GenericResourceRef& other)     = default;
        GenericResourceRef& operator=(GenericResourceRef&& other) noexcept = default;

        ~GenericResourceRef() override = default;

        operator IResource*() const;
        IResource* operator*() const;
        IResource* operator->() const;

        bool hasValue() const override;

        bool serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override;
        bool deserialize(const rapidjson::Value& json) override;

        bool   serialize(std::vector<char>& output) const override;
        size_t deserialize(const void* data, size_t length) override;

    private:
        std::string m_type;
    };
}

#include "PantheonCore/Resources/ResourceRef.inl"
