#include "PantheonRendering/RHI/Null/NullShader.h"

namespace PantheonRendering::RHI
{
    bool NullShader::load(const std::string&)
    {
        return true;
    }

    bool NullShader::init()
    {
        return true;
    }

    bool NullShader::serialize(std::vector<char>& output) const
    {
        output.resize(output.size() + 1); // Can't not write anything. Deserialization considers 0 as failure
        return true;
    }

    size_t NullShader::deserialize(const void* data, const size_t length)
    {
        return data != nullptr && length > 0 ? 1 : 0;
    }

    void NullShader::bind()
    {
    }

    void NullShader::unbind()
    {
    }

    const IShader::UniformInfo& NullShader::getUniformInfo(const std::string&)
    {
        static UniformInfo dummy;
        return dummy;
    }

    void NullShader::setUniformInt(const std::string&, int)
    {
    }

    void NullShader::setUniformUInt(const std::string&, uint32_t)
    {
    }

    void NullShader::setUniformFloat(const std::string&, float)
    {
    }

    void NullShader::setUniformVec2(const std::string&, const LibMath::Vector2&)
    {
    }

    void NullShader::setUniformVec3(const std::string&, const LibMath::Vector3&)
    {
    }

    void NullShader::setUniformVec4(const std::string&, const LibMath::Vector4&)
    {
    }

    void NullShader::setUniformMat3(const std::string&, const LibMath::Matrix3&)
    {
    }

    void NullShader::setUniformMat4(const std::string&, const LibMath::Matrix4&)
    {
    }

    void NullShader::setUniformTexture(const std::string&, ITexture*)
    {
    }

    int NullShader::getUniformInt(const std::string&)
    {
        return {};
    }

    float NullShader::getUniformFloat(const std::string&)
    {
        return {};
    }

    LibMath::Vector2 NullShader::getUniformVec2(const std::string&)
    {
        return {};
    }

    LibMath::Vector3 NullShader::getUniformVec3(const std::string&)
    {
        return {};
    }

    LibMath::Vector4 NullShader::getUniformVec4(const std::string&)
    {
        return {};
    }

    LibMath::Matrix3 NullShader::getUniformMat3(const std::string&)
    {
        return {};
    }

    LibMath::Matrix4 NullShader::getUniformMat4(const std::string&)
    {
        return {};
    }
}
