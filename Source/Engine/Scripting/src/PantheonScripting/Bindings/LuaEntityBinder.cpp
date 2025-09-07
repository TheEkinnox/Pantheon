#include "PantheonScripting/LuaScriptList.h"
#include "PantheonScripting/LuaTypeRegistry.h"
#include "PantheonScripting/Bindings/LuaECSBinder.h"

#include <PantheonCore/ECS/ComponentHandle.h>

using namespace PantheonCore::ECS;

namespace PantheonScripting::Bindings
{
    using SearchOrigin = EntityHandle::EComponentSearchOrigin;

    static bool hasComponent(const EntityHandle& self, const std::string& type)
    {
        if (!self || type.empty())
            return false;

        const LuaTypeRegistry& typeRegistry = LuaTypeRegistry::getInstance();

        if (!CHECK(typeRegistry.contains(type), "Unknown component type \"%s\"", type.c_str()))
            return false;

        return self.has(typeRegistry.getRegisteredTypeId(type));
    }

    static ComponentHandle getComponent(const EntityHandle& self, const std::string& type)
    {
        if (type.empty())
            return {};

        const LuaTypeRegistry& typeRegistry = LuaTypeRegistry::getInstance();

        if (!CHECK(typeRegistry.contains(type), "Unknown component type \"%s\"", type.c_str()))
            return {};

        return self.get(typeRegistry.getRegisteredTypeId(type));
    }

    static ComponentHandle getOrCreate(const EntityHandle& self, const std::string& type)
    {
        if (!self || type.empty())
            return {};

        const LuaTypeRegistry& typeRegistry = LuaTypeRegistry::getInstance();

        if (!CHECK(typeRegistry.contains(type), "Unknown component type \"%s\"", type.c_str()))
            return {};

        return self.getOrCreate(typeRegistry.getRegisteredTypeId(type));
    }

    static ComponentHandle getInParent(const EntityHandle& self, const std::string& type)
    {
        if (type.empty())
            return {};

        const LuaTypeRegistry& typeRegistry = LuaTypeRegistry::getInstance();

        if (!CHECK(typeRegistry.contains(type), "Unknown component type \"%s\"", type.c_str()))
            return {};

        return self.getInParent(typeRegistry.getRegisteredTypeId(type));
    }

    static ComponentHandle getInChildren(const EntityHandle& self, const std::string& type)
    {
        if (type.empty())
            return {};

        const LuaTypeRegistry& typeRegistry = LuaTypeRegistry::getInstance();

        if (!CHECK(typeRegistry.contains(type), "Unknown component type \"%s\"", type.c_str()))
            return {};

        return self.getInChildren(typeRegistry.getRegisteredTypeId(type));
    }

    static ComponentHandle getInHierarchy(const EntityHandle& self, const std::string& type, const SearchOrigin searchOrigin)
    {
        if (type.empty())
            return {};

        const LuaTypeRegistry& typeRegistry = LuaTypeRegistry::getInstance();

        if (!CHECK(typeRegistry.contains(type), "Unknown component type \"%s\"", type.c_str()))
            return {};

        return self.getInHierarchy(typeRegistry.getRegisteredTypeId(type), searchOrigin);
    }

    static void removeComponent(const EntityHandle& self, const std::string& type)
    {
        if (!self || type.empty())
            return;

        const LuaTypeRegistry& typeRegistry = LuaTypeRegistry::getInstance();

        if (!CHECK(typeRegistry.contains(type), "Unknown component type \"%s\"", type.c_str()))
            return;

        const auto typeId = typeRegistry.getRegisteredTypeId(type);
        self.remove(typeId);
    }

    static LuaScriptHandle getScript(const EntityHandle& self, const std::string& name)
    {
        const LuaScriptList* script = self.get<LuaScriptList>();
        return script ? script->get(name) : LuaScriptHandle{};
    }

    static LuaScriptHandle getScriptInParent(const EntityHandle& self, const std::string& name)
    {
        if (LuaScriptHandle script = getScript(self, name))
            return script;

        EntityHandle parent = self.getParent();

        while (parent)
        {
            if (LuaScriptHandle script = getScript(parent, name))
                return script;

            parent = parent.getParent();
        }

        return {};
    }

    static LuaScriptHandle getScriptInChildren(const EntityHandle& self, const std::string& name)
    {
        if (const LuaScriptHandle script = getScript(self, name))
            return script;

        for (const EntityHandle& child : self)
        {
            if (const LuaScriptHandle script = getScriptInChildren(child, name))
                return script;
        }

        return {};
    }

    static LuaScriptHandle getScriptInHierarchy(
        const EntityHandle& self, const std::string& name, const SearchOrigin searchOrigin)
    {
        switch (searchOrigin)
        {
        case SearchOrigin::ROOT:
        {
            return getScriptInChildren(self.getRoot(), name);
        }
        case SearchOrigin::PARENT:
        {
            if (const LuaScriptHandle script = getScriptInParent(self, name))
                return script;

            return getScriptInChildren(self, name);
        }
        case SearchOrigin::CHILDREN:
        {
            if (const LuaScriptHandle script = getScriptInChildren(self, name))
                return script;

            return getScriptInParent(self, name);
        }
        default:
            PTH_ASSERT(false, "Invalid component search origin");
            return {};
        }
    }

    void LuaECSBinder::bindEntity(sol::state& luaState)
    {
        static constexpr const char* typeName = "Entity";

        sol::usertype handleType = luaState.new_usertype<EntityHandle>(
            typeName,
            sol::meta_function::concatenation, sol::overload(
                [](const char* str, const EntityHandle& self)
                {
                    std::ostringstream oss;
                    oss << str << self;
                    return oss.str();
                },
                [](const EntityHandle& self, const char* str)
                {
                    std::ostringstream oss;
                    oss << self << str;
                    return oss.str();
                }
            ),
            "isValid", sol::readonly_property(&EntityHandle::operator bool),
            "scene", sol::readonly_property(&EntityHandle::getScene),
            "copy", &EntityHandle::copy,
            "destroy", &EntityHandle::destroy,
            "root", sol::readonly_property(&EntityHandle::getRoot),
            "parent", sol::property(&EntityHandle::getParent, sol::resolve<void(EntityHandle)>(&EntityHandle::setParent)),
            "nextSibling", sol::readonly_property(&EntityHandle::getNextSibling),
            "previousSibling", sol::readonly_property(&EntityHandle::getPreviousSibling),
            "childCount", sol::readonly_property(&EntityHandle::getChildCount),
            "getChild", &EntityHandle::getChild,
            "addChild", &EntityHandle::addChild,
            "children", sol::readonly_property(&EntityHandle::getChildren),
            "setParent", sol::overload(
                sol::resolve<void(EntityHandle)>(&EntityHandle::setParent),
                sol::resolve<void(EntityHandle, bool)>(&EntityHandle::setParent)
            ),
            "hasScript", [](const EntityHandle& self, const std::string& name) -> bool
            {
                const LuaScriptList* script = self.get<LuaScriptList>();
                return script ? script->contains(name) : false;
            },
            "getScript", &getScript,
            "getScriptInParent", &getScriptInParent,
            "getScriptInChildren", &getScriptInChildren,
            "getScriptInHierarchy", &getScriptInHierarchy,
            "addScript", [](EntityHandle& self, const std::string& name) -> LuaScriptHandle
            {
                LuaScriptList* script = self.get<LuaScriptList>();

                std::unordered_map<std::string, sol::table> scripts;

                if (!script)
                    script = &self.make<LuaScriptList>();

                return script->add(name);
            },
            "requireScript", [](EntityHandle& self, const std::string& name) -> LuaScriptHandle
            {
                LuaScriptList* scriptList = self.get<LuaScriptList>();

                std::unordered_map<std::string, sol::table> scripts;

                if (!scriptList)
                    scriptList = &self.make<LuaScriptList>();

                return scriptList->contains(name) ? scriptList->get(name) : scriptList->add(name);
            },
            "removeScript", [](EntityHandle& self, const std::string& name)
            {
                if (LuaScriptList* script = self.get<LuaScriptList>())
                    script->remove(name);
            },
            "has", sol::overload(
                &hasComponent,
                [](const EntityHandle& self, const sol::table& type)
                {
                    if (!type.valid())
                        return false;

                    return hasComponent(self, type["__type"]["name"].get_or<std::string>({}));
                }
            ),
            "get", sol::overload(
                &getComponent,
                [](const EntityHandle& self, const sol::table& type) -> ComponentHandle
                {
                    if (!type.valid())
                        return {};

                    return getComponent(self, type["__type"]["name"].get_or<std::string>({}));
                }
            ),
            "getOrCreate", sol::overload(
                &getOrCreate,
                [](const EntityHandle& self, const sol::table& type) -> ComponentHandle
                {
                    if (!type.valid())
                        return {};

                    return getOrCreate(self, type["__type"]["name"].get_or<std::string>({}));
                }
            ),
            "getInParent", sol::overload(
                &getInParent,
                [](const EntityHandle& self, const sol::table& type) -> ComponentHandle
                {
                    if (!type.valid())
                        return {};

                    return getInParent(self, type["__type"]["name"].get_or<std::string>({}));
                }
            ),
            "getInChildren", sol::overload(
                &getInChildren,
                [](const EntityHandle& self, const sol::table& type) -> ComponentHandle
                {
                    if (!type.valid())
                        return {};

                    return getInChildren(self, type["__type"]["name"].get_or<std::string>({}));
                }
            ),
            "getInHierarchy", sol::overload(
                &getInHierarchy,
                [](const EntityHandle& self, const sol::table& type, const SearchOrigin searchOrigin)
                -> ComponentHandle
                {
                    if (!type.valid())
                        return {};

                    return getInHierarchy(self, type["__type"]["name"].get_or<std::string>({}), searchOrigin);
                }
            ),
            "remove", sol::overload(
                &removeComponent,
                [](const EntityHandle& self, const sol::table& type)
                {
                    if (!type.valid())
                        return;

                    removeComponent(self, type["__type"]["name"].get_or<std::string>({}));
                },
                [](const EntityHandle& self, ComponentHandle& component)
                {
                    if (!CHECK(component.m_owner == self, "Attempted to remove component from non-owner entity"))
                        return;

                    component.destroy();
                }
            ),
            "componentCount", sol::readonly_property(&EntityHandle::getComponentCount),
            "components", sol::readonly_property(&EntityHandle::getComponentHandles)
        );

        handleType["__type"]["name"] = typeName;

        static const LuaTypeInfo& typeInfo = LuaTypeRegistry::getInstance().registerType<EntityHandle>(typeName);
        return (void)typeInfo;
    }
}
