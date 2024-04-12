#pragma once
#include "PantheonCore/ECS/ComponentRegistry.h"

#include "PantheonScripting/LuaScript.h"

#include <PantheonCore/ECS/ComponentTraits.h>
#include <PantheonCore/Resources/ResourceRef.h>

#include <sol/sol.hpp>

namespace PantheonScripting
{
    struct LuaScriptComponent
    {
        PantheonCore::Resources::ResourceRef<LuaScript> m_script;

        sol::table m_table = sol::nil;
    };

    /**
     * \brief Deserializes the lua object from json
     * \param luaState The object's lua state
     * \param json The input json data
     * \return The created object on success. A null optional otherwise
     */
    std::optional<sol::object> luaObjectFromJson(lua_State* luaState, const rapidjson::Value& json);
}

namespace PantheonCore::ECS
{
    /**
     * \brief The action to perform when a lua script component is added
     * \param entity The component's owner
     * \param component The added component
     */
    template <>
    void ComponentTraits::onAdd(EntityHandle& entity, PantheonScripting::LuaScriptComponent& component);

    /**
     * \brief The action to perform when a lua script component is removed
     * \param entity The component's owner
     * \param component The added component
     */
    template <>
    void ComponentTraits::onRemove(EntityHandle& entity, PantheonScripting::LuaScriptComponent& component);

    /**
     * \brief The action to perform before a lua script component is changed
     * \param entity The component's owner
     * \param component The added component
     */
    template <>
    void ComponentTraits::onBeforeChange(EntityHandle& entity, PantheonScripting::LuaScriptComponent& component);

    /**
     * \brief The action to perform after a lua script component was changed
     * \param entity The component's owner
     * \param component The added component
     */
    template <>
    void ComponentTraits::onChange(EntityHandle& entity, PantheonScripting::LuaScriptComponent& component);

    /**
     * \brief Serializes the given lua script component to json
     * \param component The serialized script component instance
     * \param writer The output json writer
     * \param toSerialized The entity to serialized entity map
     * \return True on success. False otherwise
     */
    template <>
    bool ComponentRegistry::toJson(const PantheonScripting::LuaScriptComponent& component,
                                   rapidjson::Writer<rapidjson::StringBuffer>&  writer, const EntitiesMap& toSerialized);

    /**
     * \brief Deserializes the lua script component from json
     * \param out The output lua script component instance
     * \param json The input json data
     * \return True on success. False otherwise
     */
    template <>
    bool ComponentRegistry::fromJson(PantheonScripting::LuaScriptComponent& out, const rapidjson::Value& json);

    /**
     * \brief Serializes the given lua table to json
     * \param component The serialized lua table
     * \param writer The output json writer
     * \param toSerialized The entity to serialized entity map
     * \return True on success. False otherwise
     */
    template <>
    bool ComponentRegistry::toJson(
        const sol::table& component, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap& toSerialized);

    /**
     * \brief Deserializes the lua table from json
     * \param out The output lua table
     * \param json The input json data
     * \return True on success. False otherwise
     */
    template <>
    bool ComponentRegistry::fromJson(sol::table& out, const rapidjson::Value& json);

    /**
     * \brief Serializes the given lua object to json
     * \param component The serialized lua object
     * \param writer The output json writer
     * \param toSerialized The entity to serialized entity map
     * \return True on success. False otherwise
     */
    template <>
    bool ComponentRegistry::toJson(
        const sol::object& component, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap& toSerialized);
}
