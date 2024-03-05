#include "PantheonRendering/RHI/IShader.h"

#include "PantheonRendering/RHI/IRenderAPI.h"
#include "PantheonRendering/RHI/Null/NullShader.h"
#include "PantheonRendering/RHI/OpenGL/OpenGLShader.h"

#include <PantheonCore/Debug/Assertion.h>
#include <PantheonCore/Debug/Logger.h>
#include <PantheonCore/Resources/ResourceManager.h>
#include <PantheonCore/Utility/utility.h>

using namespace PantheonCore::Resources;
using namespace PantheonCore::Utility;
using namespace PantheonRendering::Enums;

using namespace PantheonRendering::Enums;
using namespace PantheonRendering::RHI;

namespace PantheonCore::Resources
{
    template <>
    IShader* createResource<IShader>()
    {
        switch (IRenderAPI::getCurrent().getBackend())
        {
        case EGraphicsAPI::NONE:
            return new NullShader();
        case EGraphicsAPI::OPENGL:
            return new OpenGLShader();
        default:
            ASSERT(false, "Failed to create shader - Unsupported graphics api");
            return nullptr;
        }
    }
}

namespace PantheonRendering::RHI
{
    std::string IShader::getTokenFromType(const EShaderType shaderType)
    {
        switch (shaderType)
        {
        case EShaderType::VERTEX:
            return "vertex";
        case EShaderType::FRAGMENT:
            return "fragment";
        case EShaderType::GEOMETRY:
            return "geometry";
        case EShaderType::TESSELATION_EVALUATION:
            return "tess_evaluation";
        case EShaderType::TESSELATION_CONTROL:
            return "tess_control";
        case EShaderType::COMPUTE:
            return "compute";
        case EShaderType::UNKNOWN:
        default:
            return "unknown";
        }
    }

    const std::unordered_map<std::string, IShader::UniformInfo>& IShader::getUniforms() const
    {
        return m_uniforms;
    }

    EShaderType IShader::getTypeFromToken(std::string shaderType)
    {
        toLowerInPlace(shaderType);

        if (shaderType == "vertex" || shaderType == "vert" || shaderType == "vs")
            return EShaderType::VERTEX;

        if (shaderType == "fragment" || shaderType == "frag" || shaderType == "fs")
            return EShaderType::FRAGMENT;

        if (shaderType == "geometry" || shaderType == "geo" || shaderType == "gs")
            return EShaderType::GEOMETRY;

        if (shaderType == "tesselation_evaluation" || shaderType == "tess_evaluation" || shaderType == "tes"
            || shaderType == "tesselation_eval" || shaderType == "tess_eval")
            return EShaderType::TESSELATION_EVALUATION;

        if (shaderType == "tesselation_control" || shaderType == "tess_control" || shaderType == "tcs"
            || shaderType == "tesselation_ctrl" || shaderType == "tess_ctrl")
            return EShaderType::TESSELATION_CONTROL;

        if (shaderType == "compute" || shaderType == "com" || shaderType == "cs")
            return EShaderType::COMPUTE;

        return EShaderType::UNKNOWN;
    }
}
