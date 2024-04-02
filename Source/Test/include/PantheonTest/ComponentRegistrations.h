#pragma once
#include <PantheonCore/ECS/Components/TagComponent.h>
#include <PantheonCore/ECS/Components/Hierarchy.h>

#include <PantheonRendering/Components/LightComponent.h>
#include <PantheonRendering/Components/ModelComponent.h>
#include <PantheonRendering/LowRenderer/Camera.h>

REGISTER_COMPONENT_TYPE(Hierarchy, PantheonCore::ECS::HierarchyComponent)
REGISTER_COMPONENT_TYPE(Tag, PantheonCore::ECS::TagComponent)
REGISTER_COMPONENT_TYPE(Transform, LibMath::Transform)

REGISTER_COMPONENT_TYPE(Light, PantheonRendering::Components::LightComponent)
REGISTER_COMPONENT_TYPE(Model, PantheonRendering::Components::ModelComponent)
REGISTER_COMPONENT_TYPE(Camera, PantheonRendering::LowRenderer::Camera)
