#include "PantheonRendering/RHI/OpenGL/OpenGLShader.h"

#include "PantheonRendering/RHI/ITexture.h"

#include <PantheonCore/Debug/Assertion.h>
#include <PantheonCore/Debug/Logger.h>
#include <PantheonCore/Resources/ResourceManager.h>
#include <PantheonCore/Utility/utility.h>

#include <sstream>

#include <glad/gl.h>

using namespace PantheonCore::Resources;
using namespace PantheonCore::Utility;
using namespace PantheonRendering::Enums;

namespace PantheonRendering::RHI
{
    GLenum toGLEnum(const EShaderType shaderType)
    {
        switch (shaderType)
        {
        case EShaderType::VERTEX:
            return GL_VERTEX_SHADER;
        case EShaderType::FRAGMENT:
            return GL_FRAGMENT_SHADER;
        case EShaderType::GEOMETRY:
            return GL_GEOMETRY_SHADER;
        case EShaderType::TESSELATION_EVALUATION:
            return GL_TESS_EVALUATION_SHADER;
        case EShaderType::TESSELATION_CONTROL:
            return GL_TESS_CONTROL_SHADER;
        case EShaderType::COMPUTE:
            return GL_COMPUTE_SHADER;
        case EShaderType::UNKNOWN:
        default:
            return GL_INVALID_ENUM;
        }
    }

    EShaderDataType getDataType(GLenum dataType)
    {
        switch (dataType)
        {
        case GL_BOOL:
            return EShaderDataType::BOOL;
        case GL_INT:
            return EShaderDataType::INT;
        case GL_UNSIGNED_INT:
            return EShaderDataType::UNSIGNED_INT;
        case GL_FLOAT:
            return EShaderDataType::FLOAT;
        case GL_FLOAT_VEC2:
            return EShaderDataType::VEC2;
        case GL_FLOAT_VEC3:
            return EShaderDataType::VEC3;
        case GL_FLOAT_VEC4:
            return EShaderDataType::VEC4;
        case GL_FLOAT_MAT3:
            return EShaderDataType::MAT3;
        case GL_FLOAT_MAT4:
            return EShaderDataType::MAT4;
        case GL_SAMPLER_2D:
            return EShaderDataType::TEXTURE;
        default:
            ASSERT(false, "Unsupported shader data type");
            return EShaderDataType::UNKNOWN;
        }
    }

    OpenGLShader::OpenGLShader(const OpenGLShader& other)
        : IShader(other), m_source(other.m_source)
    {
        if (other.m_program != 0)
        {
            [[maybe_unused]] const bool result = parseSource();
            ASSERT(result);
        }
    }

    OpenGLShader::OpenGLShader(OpenGLShader&& other) noexcept
        : m_source(std::move(other.m_source)), m_program(other.m_program)
    {
        other.m_program = 0;
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_program);
    }

    OpenGLShader& OpenGLShader::operator=(const OpenGLShader& other)
    {
        if (&other == this)
            return *this;

        m_source = other.m_source;

        if (other.m_program != 0)
        {
            [[maybe_unused]] const bool result = parseSource();
            ASSERT(result);
        }

        return *this;
    }

    OpenGLShader& OpenGLShader::operator=(OpenGLShader&& other) noexcept
    {
        if (&other == this)
            return *this;

        m_source  = other.m_source;
        m_program = other.m_program;

        other.m_program = 0;

        return *this;
    }

    bool OpenGLShader::load(const std::string& fileName)
    {
        const std::ifstream fs(fileName);

        if (!CHECK(fs.is_open(), "Unable to open file at path \"%s\"", fileName.c_str()))
            return false;

        std::ostringstream stringStream;

        stringStream << fs.rdbuf();

        m_source = stringStream.str();
        return true;
    }

    bool OpenGLShader::init()
    {
        return CHECK(parseSource(), "Unable to initialize shader - Couldn't pare source");
    }

    bool OpenGLShader::serialize(std::vector<char>& output) const
    {
        if (m_source.empty())
            return false;

        if (!CHECK(serializeString(m_source, output), "Unable to serialize shader - Failed to copy source to buffer"))
            return false;

        return true;
    }

    size_t OpenGLShader::deserialize(const void* data, const size_t length)
    {
        m_source.clear();

        if (!CHECK(data != nullptr && length > 0, "Unable to load shader - Empty memory buffer"))
            return 0;

        if (!CHECK(deserializeString(m_source, static_cast<const char*>(data), length) != 0, "Unable to load shader from memory"))
            return 0;

        return sizeof(size_t) + m_source.size();
    }

    void OpenGLShader::bind()
    {
        glUseProgram(m_program);
    }

    void OpenGLShader::unbind()
    {
        glUseProgram(0);
    }

    const IShader::UniformInfo& OpenGLShader::getUniformInfo(const std::string& name)
    {
        const auto it = m_uniforms.find(name);

        ASSERT(it != m_uniforms.end(), "Failed to find uniform with name \"%s\"", name.c_str());
        return it->second;
    }

    void OpenGLShader::setUniformInt(const std::string& name, const int value)
    {
        glUniform1i(getUniformInfo(name).m_location, value);
    }

    void OpenGLShader::setUniformUInt(const std::string& name, const uint32_t value)
    {
        glUniform1ui(getUniformInfo(name).m_location, value);
    }

    void OpenGLShader::setUniformFloat(const std::string& name, const float value)
    {
        glUniform1f(getUniformInfo(name).m_location, value);
    }

    void OpenGLShader::setUniformVec2(const std::string& name, const LibMath::Vector2& value)
    {
        glUniform2fv(getUniformInfo(name).m_location, 1, value.getArray());
    }

    void OpenGLShader::setUniformVec3(const std::string& name, const LibMath::Vector3& value)
    {
        glUniform3fv(getUniformInfo(name).m_location, 1, value.getArray());
    }

    void OpenGLShader::setUniformVec4(const std::string& name, const LibMath::Vector4& value)
    {
        glUniform4fv(getUniformInfo(name).m_location, 1, value.getArray());
    }

    void OpenGLShader::setUniformMat3(const std::string& name, const LibMath::Matrix3& value)
    {
        glUniformMatrix3fv(getUniformInfo(name).m_location, 1, GL_TRUE, value.getArray());
    }

    void OpenGLShader::setUniformMat4(const std::string& name, const LibMath::Matrix4& value)
    {
        glUniformMatrix4fv(getUniformInfo(name).m_location, 1, GL_TRUE, value.getArray());
    }

    void OpenGLShader::setUniformTexture(const std::string& name, ITexture* value)
    {
        if (value)
            value->bind(static_cast<uint8_t>(getUniformInt(name)));
        else
            glBindTextureUnit(static_cast<uint8_t>(getUniformInt(name)), 0);
    }

    int OpenGLShader::getUniformInt(const std::string& name)
    {
        int value;
        glGetUniformiv(m_program, getUniformInfo(name).m_location, &value);
        return value;
    }

    float OpenGLShader::getUniformFloat(const std::string& name)
    {
        float value;
        glGetUniformfv(m_program, getUniformInfo(name).m_location, &value);
        return value;
    }

    LibMath::Vector2 OpenGLShader::getUniformVec2(const std::string& name)
    {
        LibMath::Vector2 out;
        glGetUniformfv(m_program, getUniformInfo(name).m_location, out.getArray());
        return out;
    }

    LibMath::Vector3 OpenGLShader::getUniformVec3(const std::string& name)
    {
        LibMath::Vector3 out;
        glGetUniformfv(m_program, getUniformInfo(name).m_location, out.getArray());
        return out;
    }

    LibMath::Vector4 OpenGLShader::getUniformVec4(const std::string& name)
    {
        LibMath::Vector4 out;
        glGetUniformfv(m_program, getUniformInfo(name).m_location, out.getArray());
        return out;
    }

    LibMath::Matrix3 OpenGLShader::getUniformMat3(const std::string& name)
    {
        LibMath::Matrix3 out;
        glGetUniformfv(m_program, getUniformInfo(name).m_location, out.getArray());
        return out;
    }

    LibMath::Matrix4 OpenGLShader::getUniformMat4(const std::string& name)
    {
        LibMath::Matrix4 out;
        glGetUniformfv(m_program, getUniformInfo(name).m_location, out.getArray());
        return out;
    }

    bool OpenGLShader::processIncludes(std::string& source)
    {
        if (source.empty())
            return true;

        std::string        line;
        std::istringstream sourceStream(source);
        ptrdiff_t          offset = 0;

        while (std::getline(sourceStream, line))
        {
            if (!line.starts_with("#include "))
                continue;

            const size_t lineSize = line.size();
            const size_t curPos   = !sourceStream.eof() ? static_cast<size_t>(sourceStream.tellg()) - 1 : source.size();
            const size_t startPos = curPos - lineSize;

            line.erase(0, 9); // Removes the "#include"
            const auto trimCallback = [](const char c)
            {
                return std::isspace(c) || c == '"' || c == '<' || c == '>';
            };

            trimString(line, trimCallback);

            if (!CHECK(!line.empty(), "Empty shader include path", line.c_str()))
                return false;

            std::ifstream sourceFile(line, std::ios::binary | std::ios::ate);

            if (!CHECK(sourceFile.is_open(), "Invalid shader include path: \"%s\"", line.c_str()))
                return false;

            const std::ifstream::pos_type length = sourceFile.tellg();
            sourceFile.seekg(0, std::ios::beg);

            std::string includedShader(length, 0);
            sourceFile.read(includedShader.data(), length);
            sourceFile.close();

            if (!processIncludes(includedShader))
                return false;

            source.replace(startPos + offset, lineSize, includedShader);
            offset += static_cast<ptrdiff_t>(includedShader.size()) - lineSize;
        }

        return true;
    }

    uint32_t OpenGLShader::processSource(std::string& source)
    {
        if (source.empty())
            return 0;

        std::istringstream iStrStream(source);
        std::string        token;

        iStrStream >> token;
        const EShaderType shaderType = getTypeFromToken(token);

        if (shaderType == EShaderType::UNKNOWN)
            return 0;

        std::string firstLine;
        std::getline(iStrStream, firstLine);
        source.erase(0, token.size() + firstLine.size());

        return compileSource(shaderType, source);
    }

    GLuint OpenGLShader::compileSource(const EShaderType shaderType, std::string& source)
    {
        const GLuint shaderId = glCreateShader(toGLEnum(shaderType));

        processIncludes(source);
        const char* shaderSource = source.c_str();
        const auto  sourceSize   = static_cast<GLint>(source.size());

        glShaderSource(shaderId, 1, &shaderSource, &sourceSize);
        glCompileShader(shaderId);

        int success;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            char infoLog[INFO_LOG_SIZE];
            glGetShaderInfoLog(shaderId, INFO_LOG_SIZE, nullptr, infoLog);
            DEBUG_LOG_ERROR("ERROR::SHADER::%s::COMPILATION_FAILED\n%s", getTokenFromType(shaderType).c_str(), infoLog);
            glDeleteShader(shaderId);
            return 0;
        }

        return shaderId;
    }

    bool OpenGLShader::parseSource()
    {
        reset();

        if (m_source.empty())
            return false;

        m_program = glCreateProgram();

        std::vector<std::string> sources = splitString(m_source, "#shader ", true);

        std::vector<GLuint> shaderIds;
        shaderIds.reserve(sources.size());

        for (std::string& source : sources)
        {
            if (const uint32_t id = processSource(source))
            {
                shaderIds.push_back(id);
                glAttachShader(m_program, id);
            }
        }

        const bool isSuccess = link();

        for (const GLuint shaderId : shaderIds)
            glDeleteShader(shaderId);

        cacheUniforms();

        return isSuccess;
    }

    bool OpenGLShader::link() const
    {
        if (m_program == 0)
            return false;

        int success;
        glLinkProgram(m_program);
        glGetProgramiv(m_program, GL_LINK_STATUS, &success);

        if (!success)
        {
            char infoLog[INFO_LOG_SIZE];
            glGetProgramInfoLog(m_program, INFO_LOG_SIZE, nullptr, infoLog);
            DEBUG_LOG_ERROR("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", infoLog);
            return false;
        }

        glValidateProgram(m_program);
        glGetProgramiv(m_program, GL_VALIDATE_STATUS, &success);

        if (!success)
        {
            char infoLog[INFO_LOG_SIZE];
            glGetProgramInfoLog(m_program, INFO_LOG_SIZE, nullptr, infoLog);
            DEBUG_LOG_ERROR("ERROR::SHADER::PROGRAM::VALIDATION_FAILED\n%s", infoLog);
            return false;
        }

        return true;
    }

    void OpenGLShader::cacheUniforms()
    {
        m_uniforms.clear();

        if (!glIsProgram(m_program))
            return;

        int uniformCount;
        glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &uniformCount);

        int maxLength;
        glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);

        if (uniformCount <= 0 || maxLength <= 0)
            return;

        m_uniforms.reserve(uniformCount);

        std::string nameBuffer(maxLength, '\0');
        for (int i = 0; i < uniformCount; ++i)
        {
            int    length, size;
            GLenum dataType;

            glGetActiveUniform(m_program, i, maxLength, &length, &size, &dataType, nameBuffer.data());
            ASSERT(length > 0, "Failed to read uniform name");

            const std::string name = nameBuffer.substr(0, length);

            m_uniforms[name] = { getDataType(dataType), glGetUniformLocation(m_program, name.c_str()) };
        }
    }

    void OpenGLShader::reset()
    {
        glDeleteProgram(m_program);
        m_program = 0;
        m_uniforms.clear();
    }
}
