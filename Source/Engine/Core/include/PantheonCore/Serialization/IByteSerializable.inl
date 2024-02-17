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

#ifdef __LIBMATH__VECTOR__VECTOR2_H__
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
#endif

#ifdef __LIBMATH__VECTOR__VECTOR3_H__
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
#endif

#ifdef __LIBMATH__QUATERNION_H__
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
#endif
}
