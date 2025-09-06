#include "PantheonScripting/Bindings/LuaECSBinder.h"

#include "PantheonScripting/LuaScriptHandle.h"
#include "PantheonScripting/LuaTypeRegistry.h"

#include <PantheonCore/ECS/ComponentHandle.h>
#include <PantheonCore/ECS/Components/TagComponent.h>

#include <sol/state.hpp>

using namespace PantheonCore::ECS;
using namespace PantheonCore::Utility;

namespace PantheonScripting::Bindings
{
    static void bindTag(sol::state&);
    static void bindScene(sol::state&);
    static void bindComponent(sol::state&);
    static void bindScriptHandle(sol::state&);
    static void bindComponentSearchOrigin(sol::state&);

    void LuaECSBinder::bind(sol::state& luaState)
    {
        bindTag(luaState);
        bindScene(luaState);
        bindEntity(luaState);
        bindComponent(luaState);
        bindScriptHandle(luaState);
        bindComponentSearchOrigin(luaState);
    }

    static void bindTag(sol::state& luaState)
    {
        static constexpr const char* typeName = "Tag";

        sol::usertype tagType = luaState.new_usertype<TagComponent>(
            typeName,
            sol::meta_function::construct, sol::overload(
                []
                {
                    return TagComponent{};
                },
                [](const std::string& tag)
                {
                    return TagComponent{ tag };
                }
            ),
            sol::meta_function::concatenation, sol::overload(
                [](const char* str, const TagComponent& self)
                {
                    std::ostringstream oss;
                    oss << str << self.m_tag;
                    return oss.str();
                },
                [](const TagComponent& self, const char* str)
                {
                    std::ostringstream oss;
                    oss << self.m_tag << str;
                    return oss.str();
                }
            ),
            "value", &TagComponent::m_tag
        );

        tagType["__type"]["name"] = typeName;

        static const LuaTypeInfo& typeInfo = LuaTypeRegistry::getInstance().registerType<TagComponent>(typeName);
        return (void)typeInfo;
    }

    static void bindScene(sol::state& luaState)
    {
        static constexpr const char* typeName = "Scene";

        sol::usertype sceneType = luaState.new_usertype<Scene>(
            typeName,
            sol::meta_function::construct, sol::no_constructor,
            "create", sol::overload(
                sol::resolve<EntityHandle()>(&Scene::create),
                sol::resolve<EntityHandle(Entity)>(&Scene::create),
                [](Scene& self, const EntityHandle& hint)
                {
                    return self.create(hint);
                }
            ),
            "destroy", sol::overload(
                &Scene::destroy,
                [](Scene& self, const EntityHandle& handle)
                {
                    return self.destroy(handle);
                }
            ),
            "isValid", sol::overload(
                &Scene::isValid,
                [](const Scene& self, const EntityHandle& handle)
                {
                    return self.isValid(handle);
                }
            ),
            "entities", sol::readonly_property(
                [](Scene& self) -> std::vector<EntityHandle>
                {
                    std::vector<EntityHandle> out;

                    const auto& storage = self.getStorage<Entity>();
                    out.reserve(storage.size());

                    for (const auto& entity : storage)
                        out.emplace_back(&self, entity);

                    return out;
                }
            ),
            "contains", sol::overload(
                &Scene::contains,
                [](const Scene& self, const EntityHandle& handle)
                {
                    return self.contains(handle);
                }
            )
        );

        sceneType["__type"]["name"] = typeName;

        static const LuaTypeInfo& typeInfo = LuaTypeRegistry::getInstance().registerType<Scene>(typeName);
        return (void)typeInfo;
    }

    static void bindComponent(sol::state& luaState)
    {
        static constexpr const char* typeName = "Component";

        sol::usertype componentType = luaState.new_usertype<ComponentHandle>(
            typeName,
            "isValid", sol::readonly_property(&ComponentHandle::operator bool),
            sol::meta_function::index, [&luaState](const ComponentHandle& self, const sol::object& index) -> sol::object
            {
                if (!self.m_owner)
                    return sol::lua_nil;

                void* component = self.get();
                return LuaTypeRegistry::getInstance().getTypeInfo(self.m_typeId).toLua(component, luaState)[index];
            },
            sol::meta_function::new_index,
            [&luaState](const ComponentHandle& self, const sol::object& key, const sol::object& value)
            {
                if (!self.m_owner)
                    return;

                void* component = self.get();

                if (!component)
                    return;

                const LuaTypeInfo& typeInfo = LuaTypeRegistry::getInstance().getTypeInfo(self.m_typeId);

                sol::userdata data = typeInfo.toLua(component, luaState);

                data[key] = value;
                typeInfo.fromLua(component, data);
            },
            "set", [](const ComponentHandle& self, const sol::userdata& value)
            {
                if (!self.m_owner)
                    return;

                void* component = self.m_owner.getScene()->getStorage(self.m_typeId).findRaw(self.m_owner);

                if (!component)
                    return;

                const LuaTypeInfo& typeInfo = LuaTypeRegistry::getInstance().getTypeInfo(self.m_typeId);
                typeInfo.fromLua(component, value);
            }
        );

        componentType["__type"]["name"] = typeName;

        static const LuaTypeInfo& typeInfo = LuaTypeRegistry::getInstance().registerType<ComponentHandle>(typeName);
        return (void)typeInfo;
    }

    static void bindScriptHandle(sol::state& luaState)
    {
        static constexpr const char* typeName = "Script";

        sol::usertype componentType = luaState.new_usertype<LuaScriptHandle>(
            typeName,
            "isValid", sol::readonly_property(&LuaScriptHandle::operator bool),
            sol::meta_function::index, [](const LuaScriptHandle& self, const sol::object& index) -> sol::object
            {
                if (!self)
                    return sol::lua_nil;

                const sol::optional<sol::object> out = self.m_table[index];
                return out.value_or(sol::lua_nil);
            },
            sol::meta_function::new_index, [](LuaScriptHandle& self, const sol::object& key, const sol::object& value)
            {
                if (self)
                    self.m_table[key] = value;
            }
        );

        componentType["__type"]["name"] = typeName;

        static const LuaTypeInfo& typeInfo = LuaTypeRegistry::getInstance().registerType<LuaScriptHandle>(typeName);
        return (void)typeInfo;
    }

    static void bindComponentSearchOrigin(sol::state& luaState)
    {
        using ESearchOrigin = EntityHandle::EComponentSearchOrigin;
        static constexpr const char* typeName = "EComponentSearchOrigin";

        luaState.new_enum(typeName,
            "ROOT", ESearchOrigin::ROOT,
            "PARENT", ESearchOrigin::PARENT,
            "CHILDREN", ESearchOrigin::CHILDREN
        );

        static const LuaTypeInfo& typeInfo = LuaTypeRegistry::getInstance().registerType<ESearchOrigin>(typeName);
        return (void)typeInfo;
    }
}
