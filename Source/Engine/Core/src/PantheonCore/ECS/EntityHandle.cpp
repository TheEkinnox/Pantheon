#include "PantheonCore/ECS/EntityHandle.h"

#include "PantheonCore/ECS/Scene.h"
#include "PantheonCore/ECS/Components/Hierarchy.h"

namespace PantheonCore::ECS
{
    EntityHandle::EntityHandle(Scene* scene, const Entity entity)
        : m_scene(scene), m_entity(entity)
    {
    }

    EntityHandle::operator bool() const
    {
        return m_scene && m_scene->isValid(m_entity);
    }

    EntityHandle::operator Entity() const
    {
        return getEntity();
    }

    Entity EntityHandle::getEntity() const
    {
        return m_entity;
    }

    Scene* EntityHandle::getScene() const
    {
        return m_scene;
    }

    EntityHandle EntityHandle::getRoot() const
    {
        EntityHandle parent = *this;
        EntityHandle root;

        do
        {
            root   = parent;
            parent = parent.getParent();
        }
        while (parent);

        return root;
    }

    EntityHandle EntityHandle::getParent() const
    {
        const HierarchyComponent* hierarchy = get<HierarchyComponent>();

        return { m_scene, hierarchy ? hierarchy->getParent() : NULL_ENTITY };
    }

    void EntityHandle::setParent(const EntityHandle parent)
    {
        HierarchyComponent* hierarchy = get<HierarchyComponent>();

        if (!hierarchy)
        {
            make<HierarchyComponent>(parent);
            return;
        }

        hierarchy->setParent(parent);
        set<HierarchyComponent>(*hierarchy);
    }

    EntityHandle EntityHandle::getNextSibling() const
    {
        const HierarchyComponent* hierarchy = get<HierarchyComponent>();

        return { m_scene, hierarchy ? hierarchy->getNextSibling() : NULL_ENTITY };
    }

    EntityHandle EntityHandle::getPreviousSibling() const
    {
        const HierarchyComponent* hierarchy = get<HierarchyComponent>();

        return { m_scene, hierarchy ? hierarchy->getPreviousSibling() : NULL_ENTITY };
    }

    size_t EntityHandle::getChildCount() const
    {
        const HierarchyComponent* hierarchy = get<HierarchyComponent>();

        return hierarchy ? hierarchy->getChildCount() : 0;
    }

    EntityHandle EntityHandle::getChild(size_t index) const
    {
        const HierarchyComponent* hierarchy = get<HierarchyComponent>();

        if (!hierarchy || hierarchy->getChildCount() <= index)
            return { m_scene, NULL_ENTITY };

        EntityHandle child = { m_scene, hierarchy->getFirstChild() };

        while (index-- > 0)
            child = child.getNextSibling();

        return child;
    }

    std::vector<EntityHandle> EntityHandle::getChildren() const
    {
        const HierarchyComponent* hierarchy = get<HierarchyComponent>();

        if (!hierarchy || hierarchy->getChildCount() == 0)
            return {};

        std::vector<EntityHandle> children;
        EntityHandle              child = { m_scene, hierarchy->getFirstChild() };

        while (child)
        {
            children.push_back(child);
            child = child.getNextSibling();
        }

        return children;
    }

    EntityHandle EntityHandle::copy() const
    {
        if (*this)
            return *this;

        return m_scene->create(m_entity);
    }

    void EntityHandle::destroy()
    {
        if (m_scene)
            m_scene->destroy(m_entity);

        m_entity = NULL_ENTITY;
    }
}
