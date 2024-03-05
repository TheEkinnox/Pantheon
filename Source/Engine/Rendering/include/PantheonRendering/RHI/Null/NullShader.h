#pragma once
#include "PantheonRendering/RHI/IShader.h"

namespace PantheonRendering::RHI
{
    class NullShader final : public IShader
    {
    public:
        NullShader()                            = default;
        NullShader(const NullShader& other)     = default;
        NullShader(NullShader&& other) noexcept = default;
        ~NullShader() override                  = default;

        NullShader& operator=(const NullShader& other)     = default;
        NullShader& operator=(NullShader&& other) noexcept = default;

        /**
         * \brief Loads a shader from a given file
         * \param fileName The path of the source file to load
         * \return True if the shader was successfully loaded. False otherwise.
         */
        bool load(const std::string& fileName) override;

        /**
         * \brief Initializes the shader
         * \return True on success. False otherwise
         */
        bool init() override;

        /**
         * \brief Serializes the shader to a byte array
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        bool serialize(std::vector<char>& output) const override;

        /**
         * \brief Deserializes the shader from the given memory buffer
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return The number of deserialized bytes on success. 0 otherwise.
         */
        size_t deserialize(const void* data, size_t length) override;

        /**
         * \brief Binds the shader to the context.
         */
        void bind() override;

        /**
         * \brief Unbinds the currently bound shader
         */
        void unbind() override;

        /**
         * \brief Get information about the uniform with the given name
         * \param name The searched uniform's name
         * \return Information about the searched uniform
         */
        const UniformInfo& getUniformInfo(const std::string& name) override;

        /**
         * \brief Sets the value of the int uniform with the given name
         * \param name The name of the uniform
         * \param value The value of the uniform
         */
        void setUniformInt(const std::string& name, int value) override;

        /**
         * \brief Sets the value of the unsigned int uniform with the given name
         * \param name The name of the uniform
         * \param value The value of the uniform
         */
        void setUniformUInt(const std::string& name, uint32_t value) override;

        /**
         * \brief Sets the value of the float uniform with the given name
         * \param name The name of the uniform
         * \param value The value of the uniform
         */
        void setUniformFloat(const std::string& name, float value) override;

        /**
         * \brief Sets the value of the Vector2 uniform with the given name
         * \param name The name of the uniform
         * \param value The value of the uniform
         */
        void setUniformVec2(const std::string& name, const LibMath::Vector2& value) override;

        /**
         * \brief Sets the value of the Vector3 uniform with the given name
         * \param name The name of the uniform
         * \param value The value of the uniform
         */
        void setUniformVec3(const std::string& name, const LibMath::Vector3& value) override;

        /**
         * \brief Sets the value of the Vector4 uniform with the given name
         * \param name The name of the uniform
         * \param value The value of the uniform
         */
        void setUniformVec4(const std::string& name, const LibMath::Vector4& value) override;

        /**
         * \brief Sets the value of the Matrix3 uniform with the given name
         * \param name The name of the uniform
         * \param value The value of the uniform
         */
        void setUniformMat3(const std::string& name, const LibMath::Matrix3& value) override;

        /**
         * \brief Sets the value of the Matrix4 uniform with the given name
         * \param name The name of the uniform
         * \param value The value of the uniform
         */
        void setUniformMat4(const std::string& name, const LibMath::Matrix4& value) override;

        /**
         * \brief Sets the value of the Texture uniform with the given name
         * \param name The name of the uniform
         * \param value The value of the uniform
         */
        void setUniformTexture(const std::string& name, ITexture* value) override;

        /**
         * \brief Gets the value of the int uniform with the given name
         * \param name The name of the uniform
         * \return The value of the uniform
         */
        int getUniformInt(const std::string& name) override;

        /**
         * \brief Gets the value of the float uniform with the given name
         * \param name The name of the uniform
         * \return The value of the uniform
         */
        float getUniformFloat(const std::string& name) override;

        /**
         * \brief Gets the value of the Vector2 uniform with the given name
         * \param name The name of the uniform
         * \return The value of the uniform
         */
        LibMath::Vector2 getUniformVec2(const std::string& name) override;

        /**
         * \brief Gets the value of the Vector3 uniform with the given name
         * \param name The name of the uniform
         * \return The value of the uniform
         */
        LibMath::Vector3 getUniformVec3(const std::string& name) override;

        /**
         * \brief Gets the value of the Vector4 uniform with the given name
         * \param name The name of the uniform
         * \return The value of the uniform
         */
        LibMath::Vector4 getUniformVec4(const std::string& name) override;

        /**
         * \brief Gets the value of the Matrix3 uniform with the given name
         * \param name The name of the uniform
         * \return The value of the uniform
         */
        LibMath::Matrix3 getUniformMat3(const std::string& name) override;

        /**
         * \brief Gets the value of the Matrix4 uniform with the given name
         * \param name The name of the uniform
         * \return The value of the uniform
         */
        LibMath::Matrix4 getUniformMat4(const std::string& name) override;
    };
}
