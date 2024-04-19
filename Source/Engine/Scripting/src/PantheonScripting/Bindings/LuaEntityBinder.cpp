#include "PantheonScripting/LuaScriptList.h"
#include "PantheonScripting/LuaTypeRegistry.h"
#include "PantheonScripting/Bindings/LuaECSBinder.h"

#include <sol/state.hpp>

using namespace PantheonCore::ECS;

namespace PantheonScripting::Bindings
{
    void LuaECSBinder::bindEntity(sol::state& luaState)
    {
        static constexpr const char* typeName = "Entity";

        static const auto getComponent = [](const EntityHandle& self, const std::string& type) -> ComponentHandle
        {
            if (type.empty())
                return {};

            ComponentRegistry& components = ComponentRegistry::getInstance();

            if (!CHECK(components.contains(type), "Unkown component type \"%s\"", type.c_str()))
                return {};

            return { self, components.getTypeInfo(type).m_typeId };
        };

        static const auto getInParent = [](const EntityHandle& self, const std::string& type) -> ComponentHandle
        {
            if (!self || type.empty())
                return {};

            ComponentRegistry& components = ComponentRegistry::getInstance();

            if (!CHECK(components.contains(type), "Unkown component type \"%s\"", type.c_str()))
                return {};

            const auto typeId = components.getTypeInfo(type).m_typeId;

            ComponentHandle current{ self, typeId };

            if (current)
                return current;

            EntityHandle parent = self.getParent();

            while (parent)
            {
                if ((current = { parent, typeId }))
                    return current;

                parent = parent.getParent();
            }

            return {};
        };

        static const auto getInChildren = [](const EntityHandle& self, const std::string& type) -> ComponentHandle
        {
            if (!self || type.empty())
                return {};

            ComponentRegistry& components = ComponentRegistry::getInstance();

            if (!CHECK(components.contains(type), "Unkown component type \"%s\"", type.c_str()))
                return {};

            const auto typeId = components.getTypeInfo(type).m_typeId;

            ComponentHandle current{ self, typeId };

            if (current)
                return current;

            const std::vector<EntityHandle> children = self.getChildren();

            for (const EntityHandle& child : children)
            {
                if ((current = { child, typeId }))
                    return current;
            }

            return {};
        };

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
            "isValid", &EntityHandle::operator bool,
            "getScene", &EntityHandle::getScene,
            "copy", &EntityHandle::copy,
            "destroy", &EntityHandle::destroy,
            "getRoot", &EntityHandle::getRoot,
            "parent", sol::property(&EntityHandle::getParent, &EntityHandle::setParent),
            "getNextSibling", &EntityHandle::getNextSibling,
            "getPreviousSibling", &EntityHandle::getPreviousSibling,
            "getChildCount", &EntityHandle::getChildCount,
            "getChild", &EntityHandle::getChild,
            "getChildren", &EntityHandle::getChildren,
            "hasScript", [](const EntityHandle& self, const std::string& name) -> bool
            {
                const LuaScriptList* script = self.get<LuaScriptList>();
                return script ? script->contains(name) : false;
            },
            "getScript", [](const EntityHandle& self, const std::string& name) -> LuaContext::ScriptHandle
            {
                const LuaScriptList* script = self.get<LuaScriptList>();
                return script ? script->get(name) : LuaContext::ScriptHandle{};
            },
            "addScript", [](EntityHandle& self, const std::string& name) -> LuaContext::ScriptHandle
            {
                LuaScriptList* script = self.get<LuaScriptList>();

                std::unordered_map<std::string, sol::table> scripts;

                if (!script)
                    script = &self.make<LuaScriptList>();

                return script->add(name);
            },
            "removeScript", [](EntityHandle& self, const std::string& name)
            {
                if (LuaScriptList* script = self.get<LuaScriptList>())
                    script->remove(name);
            },
            "has", [](const EntityHandle& self, const std::string& type)
            {
                if (!self || type.empty())
                    return false;

                ComponentRegistry& components = ComponentRegistry::getInstance();

                if (!CHECK(components.contains(type), "Unkown component type \"%s\"", type.c_str()))
                    return false;

                const auto typeId = components.getTypeInfo(type).m_typeId;
                return self.getScene()->getStorage(typeId).contains(self.getEntity());
            },
            "get", getComponent,
            "getOrCreate", [](EntityHandle& self, const std::string& type) -> ComponentHandle
            {
                if (type.empty())
                    return {};

                ComponentRegistry& components = ComponentRegistry::getInstance();

                if (!CHECK(components.contains(type), "Unkown component type \"%s\"", type.c_str()))
                    return {};

                const ComponentTypeInfo& typeInfo = components.getTypeInfo(type);

                if (self.getScene()->getStorage(typeInfo.m_typeId).getOrCreateRaw(self))
                    return { self, typeInfo.m_typeId };

                return {};
            },
            "getInParent", getInParent,
            "getInChildren", getInChildren,
            "getInHierarchy",
            [](const EntityHandle& self, const std::string& type, EntityHandle::EComponentSearchOrigin searchOrigin)
            -> ComponentHandle
            {
                switch (searchOrigin)
                {
                case EntityHandle::EComponentSearchOrigin::ROOT:
                {
                    return getInChildren(self.getRoot(), type);
                }
                case EntityHandle::EComponentSearchOrigin::PARENT:
                {
                    if (const ComponentHandle component = getInParent(self, type))
                        return component;

                    return getInChildren(self, type);
                }
                case EntityHandle::EComponentSearchOrigin::CHILDREN:
                {
                    if (const ComponentHandle component = getInChildren(self, type))
                        return component;

                    return getInParent(self, type);
                }
                default:
                    ASSERT(false, "Invalid component search origin");
                    return {};
                }
            },
            "remove", [](EntityHandle& self, const std::string& type)
            {
                if (!self || type.empty())
                    return;

                ComponentRegistry& components = ComponentRegistry::getInstance();

                if (!CHECK(components.contains(type), "Unkown component type \"%s\"", type.c_str()))
                    return;

                const auto typeId = components.getTypeInfo(type).m_typeId;
                self.getScene()->getStorage(typeId).remove(self);
            },
            "getComponentCount", &EntityHandle::getComponentCount,
            "getComponents", [](EntityHandle& self) -> std::vector<ComponentHandle>
            {
                const auto ids = self.getComponentIds();

                std::vector<ComponentHandle> components;

                for (auto id : ids)
                    components.emplace_back(self, id);

                return components;
            }
        );

        handleType["__type"]["name"] = typeName;

        static const LuaTypeInfo& typeInfo = LuaTypeRegistry::getInstance().registerType<EntityHandle>(typeName);
        return (void)typeInfo;
    }
}
