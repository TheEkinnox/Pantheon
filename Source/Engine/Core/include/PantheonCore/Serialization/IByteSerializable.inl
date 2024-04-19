#pragma once
#include "PantheonCore/Serialization/IByteSerializable.h"
#include "PantheonCore/Utility/ByteOrder.h"
#include "PantheonCore/Utility/Copy.h"

namespace PantheonCore::Serialization
{
    template <typename SizeT>
    bool IByteSerializable::toBinaryWithSize(std::vector<char>& output) const
    {
        const size_t startSize = output.size();

        // Reserve the space for buffer size and initialize it at 0
        output.resize(startSize + sizeof(SizeT), 0);

        // Serialize the element after the reserved space
        if (!toBinary(output))
            return false;

        // Calculate and write the serialized element's bytes
        const SizeT writtenBytes   = static_cast<SizeT>(output.size() - (startSize + sizeof(SizeT)));
        const SizeT beWrittenBytes = Utility::toBigEndian(writtenBytes);
        if (!Utility::memCopy(output.data() + startSize, output.size() - writtenBytes, &beWrittenBytes, sizeof(SizeT)))
            return false;

        return true;
    }

    template <typename T>
    bool IByteSerializable::writeNumber(const T value, std::vector<char>& output)
    {
        const size_t startSize = output.size();
        output.resize(startSize + sizeof(T));

        const T beSize = Utility::toBigEndian(value);
        if (!Utility::memCopy(output.data() + startSize, output.size() - startSize, &beSize, sizeof(T)))
            return false;

        return true;
    }

    template <typename T, typename U>
    size_t IByteSerializable::readNumber(T& out, const char* data, const size_t length)
    {
        if (data == nullptr || length < sizeof(U))
            return 0;

        U value{};

        if (!Utility::memCopy(&value, sizeof(U), data, sizeof(U)))
            return 0;

        out = static_cast<T>(Utility::fromBigEndian(value));
        return sizeof(U);
    }

    template <typename SizeT>
    bool IByteSerializable::serializeString(const std::string& string, std::vector<char>& output)
    {
        const SizeT strLength = static_cast<SizeT>(string.size());

        size_t offset = output.size();
        output.resize(offset + sizeof(SizeT) + strLength);

        const SizeT beStrLength = Utility::toBigEndian(strLength);
        if (!Utility::memCopy(output.data() + offset, output.size() - offset, &beStrLength, sizeof(SizeT)))
            return false;

        offset += sizeof(SizeT);
        if (!Utility::memCopy(output.data() + offset, output.size() - offset, string.c_str(), strLength))
            return false;

        return true;
    }

    template <typename SizeT>
    size_t IByteSerializable::deserializeString(std::string& out, const char* data, const size_t length)
    {
        if (data == nullptr || length < sizeof(SizeT))
            return 0;

        SizeT strLength = 0;

        if (!Utility::memCopy(&strLength, sizeof(SizeT), data, sizeof(SizeT)))
            return 0;

        strLength = Utility::fromBigEndian(strLength);

        const size_t offset = sizeof(SizeT);

        if (length <= offset || length - offset < strLength)
            return 0;

        out.resize(strLength);

        if (!Utility::memCopy(out.data(), out.size(), data + offset, strLength))
            return 0;

        return sizeof(SizeT) + out.size();
    }

    inline bool IByteSerializable::serializeVector2(LibMath::Vector2 vec2, std::vector<char>& output)
    {
        vec2ToBigEndian(vec2);

        const size_t startSize = output.size();
        output.resize(startSize + sizeof(LibMath::Vector2));
        return Utility::memCopy(output.data() + startSize, output.size() - startSize, vec2.getArray(), sizeof(LibMath::Vector2));
    }

    inline size_t IByteSerializable::deserializeVector2(LibMath::Vector2& out, const char* data, const size_t length)
    {
        if (data == nullptr || length < sizeof(LibMath::Vector2))
            return 0;

        if (!Utility::memCopy(out.getArray(), sizeof(LibMath::Vector2), data, sizeof(LibMath::Vector2)))
            return 0;

        vec2FromBigEndian(out);

        return sizeof(LibMath::Vector2);
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
        return Utility::memCopy(output.data() + startSize, output.size() - startSize, vec3.getArray(), sizeof(LibMath::Vector3));
    }

    inline size_t IByteSerializable::deserializeVector3(LibMath::Vector3& out, const char* data, const size_t length)
    {
        if (data == nullptr || length < sizeof(LibMath::Vector3))
            return 0;

        if (!Utility::memCopy(out.getArray(), sizeof(LibMath::Vector3), data, sizeof(LibMath::Vector3)))
            return 0;

        vec3FromBigEndian(out);

        return sizeof(LibMath::Vector3);
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
        return Utility::memCopy(output.data() + startSize, output.size() - startSize, vec4.getArray(), sizeof(LibMath::Vector4));
    }

    inline size_t IByteSerializable::deserializeVector4(LibMath::Vector4& out, const char* data, size_t length)
    {
        if (data == nullptr || length < sizeof(LibMath::Vector4))
            return 0;

        if (!Utility::memCopy(out.getArray(), sizeof(LibMath::Vector4), data, sizeof(LibMath::Vector4)))
            return 0;

        vec4FromBigEndian(out);

        return sizeof(LibMath::Vector4);
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
        return Utility::memCopy(output.data() + startSize, output.size() - startSize, quat.getArray(), sizeof(LibMath::Quaternion));
    }

    inline size_t IByteSerializable::deserializeQuaternion(LibMath::Quaternion& out, const char* data, const size_t length)
    {
        if (data == nullptr || length < sizeof(LibMath::Quaternion))
            return 0;

        if (!Utility::memCopy(out.getArray(), sizeof(LibMath::Quaternion), data, sizeof(LibMath::Quaternion)))
            return 0;

        out.m_x = Utility::fromBigEndian(out.m_x);
        out.m_y = Utility::fromBigEndian(out.m_y);
        out.m_z = Utility::fromBigEndian(out.m_z);
        out.m_w = Utility::fromBigEndian(out.m_w);

        return sizeof(LibMath::Quaternion);
    }

    template <LibMath::length_t Rows, LibMath::length_t Cols, typename DataT>
    bool IByteSerializable::serializeMatrix(LibMath::TMatrix<Rows, Cols, DataT> matrix, std::vector<char>& output)
    {
        using MatT = LibMath::TMatrix<Rows, Cols, DataT>;

        for (size_t i = 0; i < MatT::getSize(); ++i)
            matrix[i] = Utility::toBigEndian(matrix[i]);

        const size_t startSize = output.size();
        output.resize(startSize + sizeof(MatT));
        return Utility::memCopy(output.data() + startSize, output.size() - startSize, matrix.getArray(), sizeof(MatT));
    }

    template <LibMath::length_t Rows, LibMath::length_t Cols, typename DataT>
    size_t IByteSerializable::deserializeMatrix(LibMath::TMatrix<Rows, Cols, DataT>& out, const char* data, size_t length)
    {
        using MatT = LibMath::TMatrix<Rows, Cols, DataT>;

        if (data == nullptr || length < sizeof(MatT) || !Utility::memCopy(out.getArray(), sizeof(MatT), data, sizeof(MatT)))
            return 0;

        for (size_t i = 0; i < MatT::getSize(); ++i)
            out[i]    = Utility::fromBigEndian(out[i]);

        return sizeof(MatT);
    }
}
