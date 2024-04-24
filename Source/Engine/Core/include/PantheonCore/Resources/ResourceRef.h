#pragma once
#include "PantheonCore/Resources/IResource.h"
#include "PantheonCore/Serialization/Serializer.h"

#include <type_traits>

namespace PantheonCore::Resources
{
    class ResourceRefBase
    {
    };

    template <class T>
    class ResourceRef : public ResourceRefBase
    {
        static_assert(std::is_same_v<IResource, T> || std::is_base_of_v<IResource, T>);

    public:
        using KeySizeT = uint8_t;
        using PathSizeT = uint8_t;
        using RefCountT = uint32_t;

        /**
         * \brief Creates an empty resource ref
         */
        ResourceRef() = default;

        /**
         * \brief Creates a handle to the resource with the given key and path
         * \param key The resource's key
         * \param path The resource's path
         * \param resource A pointer to the referenced resource
         */
        ResourceRef(std::string key, std::string path, T* resource);

        /**
         * \brief Creates a handle to the resource with the given key and path
         * \param key The resource's key
         * \param path The resource's path
         */
        ResourceRef(const std::string& key, const std::string& path);

        /**
         * \brief Creates a copy of the given resource reference
         * \param other The resource reference to copy
         */
        ResourceRef(const ResourceRef& other);

        /**
         * \brief Creates a move copy of the given resource reference
         * \param other The resource reference to move
         */
        ResourceRef(ResourceRef&& other) noexcept;

        /**
         * \brief Creates a copy of the given resource reference
         * \tparam U The other resource type
         * \param other The resource reference to copy
         */
        template <typename U>
        ResourceRef(const ResourceRef<U>& other);

        /**
         * \brief Creates a move copy of the given resource reference
         * \tparam U The other resource type
         * \param other The resource reference to move
         */
        template <typename U>
        ResourceRef(ResourceRef<U>&& other) noexcept;

        /**
         * \brief Destroys the resource reference
         */
        virtual ~ResourceRef();

        /**
         * \brief Assigns a copy of the given resource reference to this one
         * \param other The resource reference to copy
         * \return A reference to the modified resource reference
         */
        ResourceRef& operator=(const ResourceRef& other);

        /**
         * \brief Moves the given resource reference into this one
         * \param other The resource reference to move
         * \return A reference to the modified resource reference
         */
        ResourceRef& operator=(ResourceRef&& other) noexcept;

        /**
         * \brief Checks whether the given resource reference is equivalent to this one
         * \param other The compared resource reference
         * \return True if the other resource reference is equivalent to this one. False otherwise
         */
        bool operator==(const ResourceRef& other) const;

        /**
         * \brief Gets a reference to the referenced resource
         * \return A reference to the referenced resource
         */
        T& operator*() const;

        /**
         * \brief Gets a pointer to the referenced resource
         * \return A pointer to the referenced resource
         */
        T* operator->() const;

        /**
         * \brief Checks whether the resource reference has been set or not.
         * \return True if the resource ref is set. False otherwise.
         */
        virtual operator bool() const;

        /**
         * \brief Gets a pointer to the referenced resource
         * \return A pointer to the referenced resource
         */
        T* get() const;

        /**
         * \brief Gets a pointer to the referenced or default resource
         * \return A pointer to the referenced or default resource
         */
        T* getOrDefault() const;

        /**
         * \brief Gets the number of active references to the resource
         * \return The number of active references to the resource
         */
        RefCountT getReferenceCount() const;

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
         * \brief Resets the resource reference
         */
        void reset();

        /**
         * \brief Serializes the resource reference to a byte array
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        virtual bool toBinary(std::vector<char>& output) const;

        /**
         * \brief Deserializes the resource reference from the given memory buffer
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return The number of deserialized bytes on success. 0 otherwise.
         */
        virtual size_t fromBinary(const char* data, size_t length);

        /**
         * \brief Serializes the resource reference to json
         * \param writer The output json writer
         * \return True on success. False otherwise.
         */
        virtual bool toJson(Serialization::JsonWriter& writer) const;

        /**
         * \brief Deserializes the resource reference from json
         * \param json The input json data
         * \return True on success. False otherwise.
         */
        virtual bool fromJson(const Serialization::JsonValue& json);

    protected:
        template <typename U>
        friend class ResourceRef;

        std::string m_key;
        std::string m_path;
        T*          m_resource = nullptr;
        RefCountT*  m_refCount = nullptr;
    };

    class GenericResourceRef final : public ResourceRef<IResource>
    {
    public:
        using TypeSizeT = uint8_t;

        GenericResourceRef() = default;
        GenericResourceRef(std::string type, const std::string& key, const std::string& path, IResource* resource);
        GenericResourceRef(const std::string& type, const std::string& key, const std::string& path);
        GenericResourceRef(const GenericResourceRef& other)     = default;
        GenericResourceRef(GenericResourceRef&& other) noexcept = default;

        template <typename T>
        GenericResourceRef(const ResourceRef<T>& other);

        template <typename T>
        GenericResourceRef(ResourceRef<T>&& other) noexcept;

        template <typename T>
        GenericResourceRef(const ResourceRef<T>& other, std::string type);

        template <typename T>
        GenericResourceRef(ResourceRef<T>&& other, std::string type) noexcept;

        ~GenericResourceRef() override = default;

        GenericResourceRef& operator=(const GenericResourceRef& other)     = default;
        GenericResourceRef& operator=(GenericResourceRef&& other) noexcept = default;

        operator bool() const override;

        std::string getType() const;

        /**
         * \brief Serializes the generic resource reference to a byte array
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        bool toBinary(std::vector<char>& output) const override;

        /**
         * \brief Deserializes the generic resource reference from the given memory buffer
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return The number of deserialized bytes on success. 0 otherwise.
         */
        size_t fromBinary(const char* data, size_t length) override;

        /**
         * \brief Serializes the generic resource reference to json
         * \param writer The output json writer
         * \return True on success. False otherwise.
         */
        bool toJson(Serialization::JsonWriter& writer) const override;

        /**
         * \brief Deserializes the generic resource reference from json
         * \param json The input json data
         * \return True on success. False otherwise.
         */
        bool fromJson(const Serialization::JsonValue& json) override;

    private:
        std::string m_type;
    };
}

#include "PantheonCore/Resources/ResourceRef.inl"
