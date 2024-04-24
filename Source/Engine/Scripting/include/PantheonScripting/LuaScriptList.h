#pragma once
#include "PantheonScripting/LuaContext.h"

#include <PantheonCore/ECS/ComponentRegistry.h>
#include <PantheonCore/ECS/ComponentTraits.h>
#include <PantheonCore/Resources/ResourceRef.h>

#include <string>
#include <unordered_map>

#include <sol/sol.hpp>

namespace PantheonScripting
{
    class LuaScriptList
    {
    public:
        /**
         * \brief Creates an empty lua script list
         */
        LuaScriptList() = default;

        /**
         * \brief Creates a copy of the given lua script list
         * \param other The script list to copy
         */
        LuaScriptList(const LuaScriptList& other) = default;

        /**
         * \brief Creates a move copy of the given lua script list
         * \param other The script list to move
         */
        LuaScriptList(LuaScriptList&& other) noexcept = default;

        /**
         * \brief Destroys the lua script list
         */
        ~LuaScriptList();

        /**
         * \brief Assigns a copy of the given lua script list to this one
         * \param other The script list to copy
         * \return A reference to the modified script list
         */
        LuaScriptList& operator=(const LuaScriptList& other);

        /**
         * \brief Moves the given lua script list into this one
         * \param other The script list to move
         * \return A reference to the modified script list
         */
        LuaScriptList& operator=(LuaScriptList&& other) noexcept;

        /**
         * \brief Checks whether the given lua script is in the list or not
         * \param script The script to search for
         * \return True if the script was found. False otherwise
         */
        bool contains(const std::string& script) const;

        /**
         * \brief Gets the given lua script's handle
         * \param script The script to get
         * \return A handle to the found script on success. An empty handle otherwise
         */
        LuaScriptHandle get(const std::string& script) const;

        /**
         * \brief Adds the given lua script to the list
         * \param script The script to add
         * \param hint The added script's base table
         * \return A handle to the added script on success. An empty handle otherwise
         */
        LuaScriptHandle add(std::string script, const sol::table& hint = sol::nil);

        /**
         * \brief Removes the given lua script from the list
         * \param script The script to remove
         */
        void remove(const std::string& script);

        /**
         * \brief Clears the lua script list
         */
        void clear();

    private:
        friend class PantheonCore::ECS::ComponentRegistry;
        friend struct PantheonCore::ECS::ComponentTraits;

        PantheonCore::ECS::EntityHandle                      m_owner;
        std::unordered_map<std::string, sol::table /*hint*/> m_scripts;
    };

    /**
     * \brief Deserializes the lua object from json
     * \param luaState The object's lua state
     * \param json The input json data
     * \param scene The object's target scene
     * \return The created object on success. A null optional otherwise
     */
    sol::optional<sol::object> luaObjectFromJson(
        lua_State* luaState, const PantheonCore::Serialization::JsonValue& json, PantheonCore::ECS::Scene* scene);

    /**
     * \brief Deserializes the lua object from binary
     * \param luaState The object's lua state
     * \param data The input memory buffer
     * \param length The input memory buffer's length
     * \param readBytes The output variable for the number of deserialized bytes
     * \param scene The object's target scene
     * \return The number of read bytes on success. 0 otherwise
     */
    sol::optional<sol::object> luaObjectFromBinary(
        lua_State* luaState, const char* data, size_t length, size_t& readBytes, PantheonCore::ECS::Scene* scene);
}

namespace PantheonCore::ECS
{
    /**
     * \brief The action to perform when a lua script component is added
     * \param entity The component's owner
     * \param component The added component
     */
    template <>
    void ComponentTraits::onAdd(EntityHandle& entity, PantheonScripting::LuaScriptList& component);

    /**
     * \brief The action to perform when a lua script component is removed
     * \param entity The component's owner
     * \param component The added component
     */
    template <>
    void ComponentTraits::onRemove(EntityHandle& entity, PantheonScripting::LuaScriptList& component);

    /**
     * \brief The action to perform before a lua script component is changed
     * \param entity The component's owner
     * \param component The added component
     */
    template <>
    void ComponentTraits::onBeforeChange(EntityHandle& entity, PantheonScripting::LuaScriptList& component);

    /**
     * \brief The action to perform after a lua script component was changed
     * \param entity The component's owner
     * \param component The added component
     */
    template <>
    void ComponentTraits::onChange(EntityHandle& entity, PantheonScripting::LuaScriptList& component);

    /**
     * \brief Serializes the given lua script component to json
     * \param component The serialized script component instance
     * \param writer The output json writer
     * \param toSerialized The entity to serialized entity map
     * \return True on success. False otherwise
     */
    template <>
    bool ComponentRegistry::toJson(
        const PantheonScripting::LuaScriptList& component, Serialization::JsonWriter& writer, const EntitiesMap& toSerialized);

    /**
     * \brief Deserializes the lua script component from json
     * \param out The output lua script component instance
     * \param json The input json data
     * \return True on success. False otherwise
     */
    template <>
    bool ComponentRegistry::fromJson(PantheonScripting::LuaScriptList& out, const Serialization::JsonValue& json, Scene* scene);

    /**
     * \brief Serializes the given lua script component to binary
     * \param component The serialized script component instance
     * \param out The output memory buffer
     * \param toSerialized The entity to serialized entity map
     * \return True on success. False otherwise
     */
    template <>
    bool ComponentRegistry::toBinary(
        const PantheonScripting::LuaScriptList& component, std::vector<char>& out, const EntitiesMap& toSerialized);

    /**
     * \brief Deserializes the lua script component from binary
     * \param out The output lua script component instance
     * \param data The input memory buffer
     * \param length The input memory buffer's length
     * \return The number of read bytes on success. 0 otherwise
     */
    template <>
    size_t ComponentRegistry::fromBinary(PantheonScripting::LuaScriptList& out, const char* data, size_t length, Scene* scene);

    /**
     * \brief Serializes the given lua table to json
     * \param component The serialized lua table
     * \param writer The output json writer
     * \param toSerialized The entity to serialized entity map
     * \return True on success. False otherwise
     */
    template <>
    bool ComponentRegistry::toJson(
        const sol::table& component, Serialization::JsonWriter& writer, const EntitiesMap& toSerialized);

    /**
     * \brief Deserializes the lua table from json
     * \param out The output lua table
     * \param json The input json data
     * \return True on success. False otherwise
     */
    template <>
    bool ComponentRegistry::fromJson(sol::table& out, const Serialization::JsonValue& json, Scene* scene);

    /**
     * \brief Serializes the given lua table to binary
     * \param component The serialized lua table
     * \param out The output memory buffer
     * \param toSerialized The entity to serialized entity map
     * \return True on success. False otherwise
     */
    template <>
    bool ComponentRegistry::toBinary(const sol::table& component, std::vector<char>& out, const EntitiesMap& toSerialized);

    /**
     * \brief Deserializes the lua table from binary
     * \param out The output lua table
     * \param data The input memory buffer
     * \param length The input memory buffer's length
     * \return The number of read bytes on success. 0 otherwise
     */
    template <>
    size_t ComponentRegistry::fromBinary(sol::table& out, const char* data, size_t length, Scene* scene);

    /**
     * \brief Serializes the given lua object to json
     * \param component The serialized lua object
     * \param writer The output json writer
     * \param toSerialized The entity to serialized entity map
     * \return True on success. False otherwise
     */
    template <>
    bool ComponentRegistry::toJson(
        const sol::object& component, Serialization::JsonWriter& writer, const EntitiesMap& toSerialized);

    /**
     * \brief Serializes the given lua object to binary
     * \param component The serialized lua object
     * \param out The output memory buffer
     * \param toSerialized The entity to serialized entity map
     * \return True on success. False otherwise
     */
    template <>
    bool ComponentRegistry::toBinary(const sol::object& component, std::vector<char>& out, const EntitiesMap& toSerialized);
}
