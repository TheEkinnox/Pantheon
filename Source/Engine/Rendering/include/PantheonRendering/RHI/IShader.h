#pragma once
#include "PantheonRendering/Enums/EShaderDataType.h"
#include "PantheonRendering/Enums/EShaderType.h"

#include <PantheonCore/Resources/IResource.h>

#include <Matrix.h>
#include <Vector.h>

#include <string>

namespace PantheonRendering::RHI
{
    class ITexture;

    class IShader : public PantheonCore::Resources::IResource
    {
        REGISTERED_RESOURCE_BODY(IShader)

    public:
        struct UniformInfo
        {
            Enums::EShaderDataType m_type;
            int                    m_location;
        };

        /**
         * \brief Creates a copy of the given shader
         * \param other The shader to copy
         */
        IShader(const IShader& other) = default;

        /**
         * \brief Creates a move copy of the given shader
         * \param other The shader to move
         */
        IShader(IShader&& other) noexcept = default;

        /**
         * \brief Destroys the shader
         */
        ~IShader() override = default;

        /**
         * \brief Assigns a copy of the given shader to this one
         * \param other The shader to copy
         * \return A reference to the modified shader
         */
        IShader& operator=(const IShader& other) = default;

        /**
         * \brief Moves the given shader into this one
         * \param other The shader to move
         * \return A reference to the modified shader
         */
        IShader& operator=(IShader&& other) noexcept = default;

        /**
         * \brief Binds the shader to the context.
         */
        virtual void bind() = 0;

        /**
         * \brief Unbinds the currently bound shader
         */
        virtual void unbind() = 0;

        /**
         * \brief Get information about the uniform with the given name
         * \param name The searched uniform's name
         * \return Information about the searched uniform
         */
        virtual const UniformInfo& getUniformInfo(const std::string& name) = 0;

        /**
         * \brief Sets the value of the int uniform with the given name
         * \param name The name of the uniform
         * \param value The value of the uniform
         */
        virtual void setUniformInt(const std::string& name, int value) = 0;

        /**
         * \brief Sets the value of the unsigned int uniform with the given name
         * \param name The name of the uniform
         * \param value The value of the uniform
         */
        virtual void setUniformUInt(const std::string& name, uint32_t value) = 0;

        /**
         * \brief Sets the value of the float uniform with the given name
         * \param name The name of the uniform
         * \param value The value of the uniform
         */
        virtual void setUniformFloat(const std::string& name, float value) = 0;

        /**
         * \brief Sets the value of the Vector2 uniform with the given name
         * \param name The name of the uniform
         * \param value The value of the uniform
         */
        virtual void setUniformVec2(const std::string& name, const LibMath::Vector2& value) = 0;

        /**
         * \brief Sets the value of the Vector3 uniform with the given name
         * \param name The name of the uniform
         * \param value The value of the uniform
         */
        virtual void setUniformVec3(const std::string& name, const LibMath::Vector3& value) = 0;

        /**
         * \brief Sets the value of the Vector4 uniform with the given name
         * \param name The name of the uniform
         * \param value The value of the uniform
         */
        virtual void setUniformVec4(const std::string& name, const LibMath::Vector4& value) = 0;

        /**
         * \brief Sets the value of the Matrix3 uniform with the given name
         * \param name The name of the uniform
         * \param value The value of the uniform
         */
        virtual void setUniformMat3(const std::string& name, const LibMath::Matrix3& value) = 0;

        /**
         * \brief Sets the value of the Matrix4 uniform with the given name
         * \param name The name of the uniform
         * \param value The value of the uniform
         */
        virtual void setUniformMat4(const std::string& name, const LibMath::Matrix4& value) = 0;

        /**
         * \brief Sets the value of the Texture uniform with the given name
         * \param name The name of the uniform
         * \param value The value of the uniform
         */
        virtual void setUniformTexture(const std::string& name, ITexture* value) = 0;

        /**
         * \brief Gets the value of the int uniform with the given name
         * \param name The name of the uniform
         * \return The value of the uniform
         */
        virtual int getUniformInt(const std::string& name) = 0;

        /**
         * \brief Gets the value of the float uniform with the given name
         * \param name The name of the uniform
         * \return The value of the uniform
         */
        virtual float getUniformFloat(const std::string& name) = 0;

        /**
         * \brief Gets the value of the Vector2 uniform with the given name
         * \param name The name of the uniform
         * \return The value of the uniform
         */
        virtual LibMath::Vector2 getUniformVec2(const std::string& name) = 0;

        /**
         * \brief Gets the value of the Vector3 uniform with the given name
         * \param name The name of the uniform
         * \return The value of the uniform
         */
        virtual LibMath::Vector3 getUniformVec3(const std::string& name) = 0;

        /**
         * \brief Gets the value of the Vector4 uniform with the given name
         * \param name The name of the uniform
         * \return The value of the uniform
         */
        virtual LibMath::Vector4 getUniformVec4(const std::string& name) = 0;

        /**
         * \brief Gets the value of the Matrix3 uniform with the given name
         * \param name The name of the uniform
         * \return The value of the uniform
         */
        virtual LibMath::Matrix3 getUniformMat3(const std::string& name) = 0;

        /**
         * \brief Gets the value of the Matrix4 uniform with the given name
         * \param name The name of the uniform
         * \return The value of the uniform
         */
        virtual LibMath::Matrix4 getUniformMat4(const std::string& name) = 0;

        /**
         * \brief Gets the shader's uniforms
         * \return The shader's uniforms
         */
        const std::unordered_map<std::string, UniformInfo>& getUniforms() const;

        /**
         * \brief Converts a shader type token string to its corresponding enum value
         * \param shaderType The target shader type
         * \return The type enum value corresponding to the given shader type token
         */
        static Enums::EShaderType getTypeFromToken(std::string shaderType);

        /**
         * \brief Converts a shader type enum value to its corresponding token string
         * \param shaderType The target shader type
         * \return The token string corresponding to the given shader type
         */
        static std::string getTokenFromType(Enums::EShaderType shaderType);

    protected:
        std::unordered_map<std::string, UniformInfo> m_uniforms;

        /**
         * \brief Creates a default shader
         */
        IShader() = default;
    };
}

namespace PantheonCore::Resources
{
    template <>
    PantheonRendering::RHI::IShader* createResource<PantheonRendering::RHI::IShader>();
}
