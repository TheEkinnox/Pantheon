#pragma once
#include "PantheonCore/Serialization/IByteSerializable.h"
#include "PantheonCore/Utility/ByteOrder.h"

namespace PantheonCore::Serialization
{
    template <typename SizeT>
    bool IByteSerializable::serializeWithSize(std::vector<char>& output) const
    {
        const size_t startSize = output.size();

        // Reserve the space for buffer size and initialize it at 0
        output.resize(startSize + sizeof(SizeT), 0);

        // Serialize the element after the reserved space
        if (!((const IByteSerializable*)this)->serialize(output))
            return false;

        // Calculate and write the serialized element's bytes
        const SizeT writtenBytes   = static_cast<SizeT>(output.size() - (startSize + sizeof(SizeT)));
        const SizeT beWrittenBytes = Utility::toBigEndian(writtenBytes);
        if (memcpy_s(output.data() + startSize, output.size() - writtenBytes, &beWrittenBytes, sizeof(SizeT)) != 0)
            return false;

        return true;
    }

    template <typename SizeT>
    bool IByteSerializable::writeNumber(const SizeT size, std::vector<char>& output)
    {
        const size_t startSize = output.size();
        output.resize(startSize + sizeof(SizeT));

        const SizeT beSize = Utility::toBigEndian(size);
        if (memcpy_s(output.data() + startSize, output.size() - startSize, &beSize, sizeof(SizeT)) != 0)
            return false;

        return true;
    }

    template <typename SizeT>
    SizeT IByteSerializable::readNumber(const void* data, const size_t length)
    {
        if (data == nullptr || length < sizeof(SizeT))
            return INVALID_ELEMENT_SIZE;

        SizeT elemSize = 0;

        if (memcpy_s(&elemSize, sizeof(SizeT), data, sizeof(SizeT)) != 0)
            return INVALID_ELEMENT_SIZE;

        return Utility::fromBigEndian(elemSize);
    }

    template <typename SizeT>
    bool IByteSerializable::serializeString(const std::string& string, std::vector<char>& output)
    {
        const SizeT strLength = static_cast<SizeT>(string.size());

        size_t offset = output.size();
        output.resize(offset + sizeof(SizeT) + strLength, 0);

        const SizeT beStrLength = Utility::toBigEndian(strLength);
        if (memcpy_s(output.data() + offset, output.size() - offset, &beStrLength, sizeof(SizeT)) != 0)
            return false;

        offset += sizeof(SizeT);
        if (memcpy_s(output.data() + offset, output.size() - offset, string.c_str(), strLength) != 0)
            return false;

        return true;
    }

    template <typename SizeT>
    size_t IByteSerializable::deserializeString(std::string& out, const char* data, const size_t length)
    {
        if (data == nullptr || length < sizeof(SizeT))
            return 0;

        SizeT strLength = 0;

        if (memcpy_s(&strLength, sizeof(SizeT), data, sizeof(SizeT)) != 0)
            return 0;

        strLength = Utility::fromBigEndian(strLength);

        const size_t offset = sizeof(SizeT);
        out.resize(strLength);

        if (length <= offset || length - offset < strLength || memcpy_s(out.data(), out.size(), data + offset, strLength) != 0)
            return 0;

        return sizeof(SizeT) + out.size();
    }

    inline bool IByteSerializable::serializeVector2(LibMath::Vector2 vec2, std::vector<char>& output)
    {
        vec2ToBigEndian(vec2);

        const size_t startSize = output.size();
        output.resize(startSize + sizeof(LibMath::Vector2));
        return memcpy_s(output.data() + startSize, output.size() - startSize, vec2.getArray(), sizeof(LibMath::Vector2)) == 0;
    }

    inline bool IByteSerializable::deserializeVector2(LibMath::Vector2& out, const char* data, const size_t length)
    {
        if (data == nullptr || length < sizeof(LibMath::Vector2))
            return false;

        if (memcpy_s(out.getArray(), sizeof(LibMath::Vector2), data, sizeof(LibMath::Vector2)) != 0)
            return false;

        vec2FromBigEndian(out);

        return true;
    }

    inline void IByteSerializable::vec2ToBigEndian(LibMath::Vector2& vec2)
    {
        vec2.m_x = Utility::toBigEndian(vec2.m_x);
        vec2.m_y = Utility::toBigEndian(vec2.m_y);
    }

    inline void IByteSerializable::vec2FromBigEndian(LibMath::Vector2& vec2)
    {
        vec2.m_x = Utility::fromBigEndian(vec2.m_x);
        vec2.m_y = Utility::fromBigEndian(vec2.m_y);
    }

    inline bool IByteSerializable::serializeVector3(LibMath::Vector3 vec3, std::vector<char>& output)
    {
        vec3ToBigEndian(vec3);

        const size_t startSize = output.size();
        output.resize(startSize + sizeof(LibMath::Vector3));
        return memcpy_s(output.data() + startSize, output.size() - startSize, vec3.getArray(), sizeof(LibMath::Vector3)) == 0;
    }

    inline bool IByteSerializable::deserializeVector3(LibMath::Vector3& out, const char* data, const size_t length)
    {
        if (data == nullptr || length < sizeof(LibMath::Vector3))
            return false;

        if (memcpy_s(out.getArray(), sizeof(LibMath::Vector3), data, sizeof(LibMath::Vector3)) != 0)
            return false;

        vec3FromBigEndian(out);

        return true;
    }

    inline void IByteSerializable::vec3ToBigEndian(LibMath::Vector3& vec3)
    {
        vec3.m_x = Utility::toBigEndian(vec3.m_x);
        vec3.m_y = Utility::toBigEndian(vec3.m_y);
        vec3.m_z = Utility::toBigEndian(vec3.m_z);
    }

    inline void IByteSerializable::vec3FromBigEndian(LibMath::Vector3& vec3)
    {
        vec3.m_x = Utility::fromBigEndian(vec3.m_x);
        vec3.m_y = Utility::fromBigEndian(vec3.m_y);
        vec3.m_z = Utility::fromBigEndian(vec3.m_z);
    }

    inline bool IByteSerializable::serializeVector4(LibMath::Vector4 vec4, std::vector<char>& output)
    {
        vec4ToBigEndian(vec4);

        const size_t startSize = output.size();
        output.resize(startSize + sizeof(LibMath::Vector4));
        return memcpy_s(output.data() + startSize, output.size() - startSize, vec4.getArray(), sizeof(LibMath::Vector4)) == 0;
    }

    inline bool IByteSerializable::deserializeVector4(LibMath::Vector4& out, const char* data, size_t length)
    {
        if (data == nullptr || length < sizeof(LibMath::Vector4))
            return false;

        if (memcpy_s(out.getArray(), sizeof(LibMath::Vector4), data, sizeof(LibMath::Vector4)) != 0)
            return false;

        vec4FromBigEndian(out);

        return true;
    }

    inline void IByteSerializable::vec4ToBigEndian(LibMath::Vector4& vec4)
    {
        vec4.m_x = Utility::toBigEndian(vec4.m_x);
        vec4.m_y = Utility::toBigEndian(vec4.m_y);
        vec4.m_z = Utility::toBigEndian(vec4.m_z);
        vec4.m_w = Utility::toBigEndian(vec4.m_w);
    }

    inline void IByteSerializable::vec4FromBigEndian(LibMath::Vector4& vec4)
    {
        vec4.m_x = Utility::fromBigEndian(vec4.m_x);
        vec4.m_y = Utility::fromBigEndian(vec4.m_y);
        vec4.m_z = Utility::fromBigEndian(vec4.m_z);
        vec4.m_w = Utility::fromBigEndian(vec4.m_w);
    }

    inline bool IByteSerializable::serializeQuaternion(LibMath::Quaternion quat, std::vector<char>& output)
    {
        quat.m_x = Utility::toBigEndian(quat.m_x);
        quat.m_y = Utility::toBigEndian(quat.m_y);
        quat.m_z = Utility::toBigEndian(quat.m_z);
        quat.m_w = Utility::toBigEndian(quat.m_w);

        const size_t startSize = output.size();
        output.resize(startSize + sizeof(LibMath::Quaternion));
        return memcpy_s(output.data() + startSize, output.size() - startSize, quat.getArray(), sizeof(LibMath::Quaternion)) == 0;
    }

    inline bool IByteSerializable::deserializeQuaternion(LibMath::Quaternion& out, const char* data, const size_t length)
    {
        if (data == nullptr || length < sizeof(LibMath::Quaternion))
            return false;

        if (memcpy_s(out.getArray(), sizeof(LibMath::Quaternion), data, sizeof(LibMath::Quaternion)) != 0)
            return false;

        out.m_x = Utility::fromBigEndian(out.m_x);
        out.m_y = Utility::fromBigEndian(out.m_y);
        out.m_z = Utility::fromBigEndian(out.m_z);
        out.m_w = Utility::fromBigEndian(out.m_w);

        return true;
    }

    template <LibMath::length_t Rows, LibMath::length_t Cols, typename DataT>
    bool IByteSerializable::serializeMatrix(LibMath::TMatrix<Rows, Cols, DataT> matrix, std::vector<char>& output)
    {
        using MatT = LibMath::TMatrix<Rows, Cols, DataT>;

        for (size_t i = 0; i < MatT::getSize(); ++i)
            matrix[i] = Utility::toBigEndian(matrix[i]);

        const size_t startSize = output.size();
        output.resize(startSize + sizeof(MatT));
        return memcpy_s(output.data() + startSize, output.size() - startSize, matrix.getArray(), sizeof(MatT)) == 0;
    }

    template <LibMath::length_t Rows, LibMath::length_t Cols, typename DataT>
    bool IByteSerializable::deserializeMatrix(LibMath::TMatrix<Rows, Cols, DataT>& out, const char* data, size_t length)
    {
        using MatT = LibMath::TMatrix<Rows, Cols, DataT>;

        if (data == nullptr || length < sizeof(MatT) || memcpy_s(out.getArray(), sizeof(MatT), data, sizeof(MatT)) != 0)
            return false;

        for (size_t i = 0; i < MatT::getSize(); ++i)
            out[i] = Utility::fromBigEndian(out[i]);

        return true;
    }
}
