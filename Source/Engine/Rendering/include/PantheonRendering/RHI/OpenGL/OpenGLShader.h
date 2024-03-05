#pragma once
#include "PantheonRendering/RHI/IShader.h"

namespace PantheonRendering::RHI
{
    class OpenGLShader final : public IShader
    {
    public:
        OpenGLShader() = default;
        OpenGLShader(const OpenGLShader& other);
        OpenGLShader(OpenGLShader&& other) noexcept;
        ~OpenGLShader() override;

        OpenGLShader& operator=(const OpenGLShader& other);
        OpenGLShader& operator=(OpenGLShader&& other) noexcept;

        /**
         * \brief Loads the source code of a shader from a given file
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
         * \brief Binds the shader to the context.\n
         * IMPORTANT: the shader program MUST have been linked
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

    private:
        std::string m_source;
        uint32_t    m_program = 0;

        static constexpr int INFO_LOG_SIZE = 512;

        /**
         * \brief Compiles the given shader source
         * \param shaderType The type of shader to compile
         * \param source The source of the shader to compile
         * \return The compiled shader's handle
         */
        static uint32_t compileSource(Enums::EShaderType shaderType, std::string& source);

        /**
         * \brief Processes includes for the given shader source
         * \param source The shader source for which includes should be processed
         * \return True on success. False otherwise
         */
        static bool processIncludes(std::string& source);

        /**
         * \brief
         * \param source The shader source to process
         * \return The created shader id
         */
        static uint32_t processSource(std::string& source);

        /**
         * \brief Parses the shader's source and create the appropriate shader types
         * \return True if at least one type of shader was extracted. False otherwise
         */
        bool parseSource();

        /**
         * \brief Links the shader program.\n
         * \return True if the shader is linked successfully. False otherwise
         */
        bool link() const;

        /**
         * \brief Caches the shader's uniforms
         */
        void cacheUniforms();

        /**
         * \brief Resets the shader program and the uniform locations cache
         */
        void reset();
    };
}
