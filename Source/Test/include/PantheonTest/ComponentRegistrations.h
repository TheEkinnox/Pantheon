#pragma once
#include <PantheonCore/ECS/Components/TagComponent.h>
#include <PantheonCore/ECS/Components/Hierarchy.h>

REGISTER_COMPONENT_TYPE(Hierarchy, PantheonCore::ECS::HierarchyComponent)
REGISTER_COMPONENT_TYPE(Tag, PantheonCore::ECS::TagComponent)
REGISTER_COMPONENT_TYPE(Transform, PantheonCore::ECS::TransformComponent)
REGISTER_COMPONENT_TYPE(Transform, LibMath::Transform)
