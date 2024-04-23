#include "PantheonScripting/LuaComponentHandle.h"
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

        static const auto getComponent = [](const EntityHandle& self, const std::string& type)
            -> LuaComponentHandle
        {
            if (type.empty())
                return {};

            ComponentRegistry& components = ComponentRegistry::getInstance();

            if (!CHECK(components.contains(type), "Unkown component type \"%s\"", type.c_str()))
                return {};

            return { self, components.getTypeInfo(type).m_typeId };
        };

        static const auto getInParent = [](const EntityHandle& self, const std::string& type)
            -> LuaComponentHandle
        {
            if (!self || type.empty())
                return {};

            ComponentRegistry& components = ComponentRegistry::getInstance();

            if (!CHECK(components.contains(type), "Unkown component type \"%s\"", type.c_str()))
                return {};

            const auto typeId = components.getTypeInfo(type).m_typeId;

            LuaComponentHandle current{ self, typeId };

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

        static const auto getInChildren = [](const EntityHandle& self, const std::string& type)
            -> LuaComponentHandle
        {
            if (!self || type.empty())
                return {};

            ComponentRegistry& components = ComponentRegistry::getInstance();

            if (!CHECK(components.contains(type), "Unkown component type \"%s\"", type.c_str()))
                return {};

            const auto typeId = components.getTypeInfo(type).m_typeId;

            LuaComponentHandle current{ self, typeId };

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
            "isValid", sol::readonly_property(&EntityHandle::operator bool),
            "scene", sol::readonly_property(&EntityHandle::getScene),
            "copy", &EntityHandle::copy,
            "destroy", &EntityHandle::destroy,
            "root", sol::readonly_property(&EntityHandle::getRoot),
            "parent", sol::property(&EntityHandle::getParent, &EntityHandle::setParent),
            "nextSibling", sol::readonly_property(&EntityHandle::getNextSibling),
            "previousSibling", sol::readonly_property(&EntityHandle::getPreviousSibling),
            "childCount", sol::readonly_property(&EntityHandle::getChildCount),
            "getChild", &EntityHandle::getChild,
            "children", sol::readonly_property(&EntityHandle::getChildren),
            "hasScript", [](const EntityHandle& self, const std::string& name) -> bool
            {
                const LuaScriptList* script = self.get<LuaScriptList>();
                return script ? script->contains(name) : false;
            },
            "getScript", [](const EntityHandle& self, const std::string& name)
            {
                const LuaScriptList* script = self.get<LuaScriptList>();
                return script ? script->get(name) : LuaScriptHandle{};
            },
            "addScript", [](EntityHandle& self, const std::string& name)
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
            "getOrCreate", [](const EntityHandle& self, const std::string& type)
            -> LuaComponentHandle
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
            [](const EntityHandle& self, const std::string& type, const EntityHandle::EComponentSearchOrigin searchOrigin)
            -> LuaComponentHandle
            {
                switch (searchOrigin)
                {
                case EntityHandle::EComponentSearchOrigin::ROOT:
                {
                    return getInChildren(self.getRoot(), type);
                }
                case EntityHandle::EComponentSearchOrigin::PARENT:
                {
                    if (const LuaComponentHandle component = getInParent(self, type))
                        return component;

                    return getInChildren(self, type);
                }
                case EntityHandle::EComponentSearchOrigin::CHILDREN:
                {
                    if (const LuaComponentHandle component = getInChildren(self, type))
                        return component;

                    return getInParent(self, type);
                }
                default:
                    ASSERT(false, "Invalid component search origin");
                    return {};
                }
            },
            "remove", [](const EntityHandle& self, const std::string& type)
            {
                if (!self || type.empty())
                    return;

                ComponentRegistry& components = ComponentRegistry::getInstance();

                if (!CHECK(components.contains(type), "Unkown component type \"%s\"", type.c_str()))
                    return;

                const auto typeId = components.getTypeInfo(type).m_typeId;
                self.getScene()->getStorage(typeId).remove(self);
            },
            "componentCount", sol::readonly_property(&EntityHandle::getComponentCount),
            "components", sol::readonly_property([](EntityHandle& self)-> std::vector<LuaComponentHandle>
            {
                const auto ids = self.getComponentIds();

                std::vector<LuaComponentHandle> components;

                for (auto id : ids)
                    components.emplace_back(self, id);

                return components;
            })
        );

        handleType["__type"]["name"] = typeName;

        static const LuaTypeInfo& typeInfo = LuaTypeRegistry::getInstance().registerType<EntityHandle>(typeName);
        return (void)typeInfo;
    }
}
