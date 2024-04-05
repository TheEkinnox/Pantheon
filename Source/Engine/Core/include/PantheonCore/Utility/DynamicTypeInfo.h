#pragma once
#include <any>
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>

namespace PantheonCore::Utility
{
    template <typename T>
    using DynamicCallResult = std::conditional_t<std::is_same_v<void, T>, bool, std::optional<T>>;

    template <class TypeInfo>
    class DynamicTypeInfo : public TypeInfo
    {
    public:
        using Callable = std::function<std::any(void* /*payload*/)>;

        /**
         * \brief Creates empty dynamic type info and initializes the base type with the given parameters
         * \tparam Args The base type's construction parameter types
         * \param args The base type's construction parameters
         */
        template <typename... Args>
        DynamicTypeInfo(Args&&... args);

        /**
         * \brief Creates a copy of the given dynamic type info
         * \param other The dynamic type info to copy
         */
        DynamicTypeInfo(const DynamicTypeInfo& other) = default;

        /**
         * \brief Creates a move copy of the given dynamic type info
         * \param other The dynamic type info to move
         */
        DynamicTypeInfo(DynamicTypeInfo&& other) = default;

        /**
         * \brief Destroys the dynamic type info
         */
        ~DynamicTypeInfo() = default;

        /**
         * \brief Assigns a copy of the given dynamic type info to this one
         * \param other The dynamic type info to copy
         * \return A reference to the modified type info
         */
        DynamicTypeInfo& operator=(const DynamicTypeInfo& other) = default;

        /**
         * \brief Moves the given dynamic type info into this one
         * \param other The dynamic type info to move
         * \return A reference to the modified type info
         */
        DynamicTypeInfo& operator=(DynamicTypeInfo&& other) = default;

        /**
         * \brief Adds a callable with the given name to the dynamic type info
         * \param name The added callable's name
         * \param callable The added callable
         * \return True on success. False otherwise (i.e.: The name was already in use)
         */
        bool add(const std::string& name, const Callable& callable);

        /**
         * \brief Adds a callable with the given name to the dynamic type info
         * \param name The added callable's name
         * \param callable The added callable
         * \return True on success. False otherwise (i.e.: The name was already in use)
         */
        void replace(const std::string& name, const Callable& callable);

        /**
         * \brief Removes the callable with the given name from the dynamic type info
         * \param name The removed callable's name
         * \return True on success. False otherwise (i.e.: The name wasn't found)
         */
        bool remove(const std::string& name);

        /**
         * \brief Checks whether a callable with the given name has been registered or not
         * \param name The searched callable's name
         * \return True if a callable with the given name was found. False otherwise
         */
        bool has(const std::string& name) const;

        /**
         * \brief Calls the callable with the given name
         * \param name The callable's name
         * \param payload The callable's payload
         * \return The result of the call
         */
        template <typename T = void>
        DynamicCallResult<T> call(const std::string& name, void* payload = nullptr) const;

    private:
        std::unordered_map<std::string /*name*/, Callable> m_callables;
    };
}

#include "PantheonCore/Utility/DynamicTypeInfo.inl"
