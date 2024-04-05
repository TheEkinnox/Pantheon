#pragma once
#include <PantheonCore/ECS/Scene.h>

#include <PantheonRendering/Resources/Material.h>
#include <PantheonRendering/Resources/Model.h>
#include <PantheonRendering/RHI/IShader.h>
#include <PantheonRendering/RHI/ITexture.h>

REGISTER_RESOURCE_TYPE(Scene, PantheonCore::ECS::Scene)

REGISTER_RESOURCE_TYPE(Material, PantheonRendering::Resources::Material)
REGISTER_RESOURCE_TYPE(Model, PantheonRendering::Resources::Model)
REGISTER_RESOURCE_TYPE(Shader, PantheonRendering::RHI::IShader)
REGISTER_RESOURCE_TYPE(Texture, PantheonRendering::RHI::ITexture)
