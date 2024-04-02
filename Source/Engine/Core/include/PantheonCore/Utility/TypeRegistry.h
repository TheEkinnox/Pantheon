#pragma once
#include <string>
#include <unordered_map>

namespace PantheonCore::Utility
{
    template <class TypeInfo>
    class TypeRegistry
    {
    public:
        using TypeId = size_t;

        /**
         * \brief Creates an empty type registry
         */
        TypeRegistry() = default;

        /**
         * \brief Creates a copy of the given type registry
         * \param other The type registry to copy
         */
        TypeRegistry(const TypeRegistry& other) = default;

        /**
         * \brief Creates a move copy of the given type registry
         * \param other The type registry to move
         */
        TypeRegistry(TypeRegistry&& other) noexcept = default;

        /**
         * \brief Destroys the type registry
         */
        ~TypeRegistry() = default;

        /**
         * \brief Assigns a copy of the given type registry to this one
         * \param other The type registry to copy
         * \return A reference to the modified type registry
         */
        TypeRegistry& operator=(const TypeRegistry& other) = default;

        /**
         * \brief Moves the given type registry into this one
         * \param other The type registry to move
         * \return A reference to the modified type registry
         */
        TypeRegistry& operator=(TypeRegistry&& other) noexcept = default;

        /**
         * \brief Registers the given type with the given info and name
         * \tparam T The registered type
         * \param name The registered type's name
         * \param info The registered type's info
         */
        template <typename T>
        void registerType(const std::string& name, const TypeInfo& info);

        /**
         * \brief Checks whether the given type has been registered or not
         * \param name The searched type name
         * \return True if the given type has been registered. False otherwise
         */
        bool contains(const std::string& name) const;

        /**
         * \brief Checks whether the given type has been registered or not
         * \param id The searched type id
         * \return True if the given type has been registered. False otherwise
         */
        bool contains(const TypeId& id) const;

        /**
         * \brief Checks whether the given type has been registered or not
         * \tparam T The searched type
         * \return True if the given type has been registered. False otherwise
         */
        template <typename T>
        bool contains() const;

        /**
         * \brief Gets the registered type information for the given component type
         * \tparam T The component type
         * \return The registered type information for the given component type
         */
        const TypeInfo& getTypeInfo(const std::string& type) const;

        /**
         * \brief Gets the registered type information for the given type id
         * \param typeId The component type's id
         * \return The registered type information for the given component type
         */
        const TypeInfo& getTypeInfo(size_t typeId) const;

        /**
         * \brief Gets the registered type information for the given type id
         * \tparam T The component type
         * \return The registered type information for the given component type
         */
        template <typename T>
        const TypeInfo& getTypeInfo() const;

        /**
         * \brief Gets the registered name for the given type id
         * \param typeId The component type's id
         * \return The registered name for the given component type
         */
        const std::string& getRegisteredTypeName(size_t typeId) const;

        /**
         * \brief Gets the registered name for the given component type
         * \tparam T The component type
         * \return The registered name for the given component type
         */
        template <typename T>
        const std::string& getRegisteredTypeName() const;

    private:
        using TypeMap = std::unordered_map<size_t, TypeInfo>;
        using TypeNameMap = std::unordered_map<size_t, std::string>;
        using TypeIdMap = std::unordered_map<std::string, size_t>;

        TypeMap     m_typeInfos;
        TypeNameMap m_typeNames;
        TypeIdMap   m_typeIds;
    };
}

#include "PantheonCore/Utility/TypeRegistry.inl"
