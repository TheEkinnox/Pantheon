#pragma once
#include "PantheonCore/Serialization/IByteSerializable.h"
#include "PantheonCore/Utility/ByteOrder.h"

namespace PantheonEngine::Core::Serialization
{
    inline bool IByteSerializable::serializeWithSize(std::vector<char>& output) const
    {
        const size_t startSize = output.size();

        // Reserve the space for buffer size and initialize it at 0
        output.resize(startSize + sizeof(ElemSizeT), 0);

        // Serialize the element after the reserved space
        if (!((const IByteSerializable*)this)->serialize(output))
            return false;

        // Calculate and write the serialized element's bytes
        const ElemSizeT writtenBytes = static_cast<ElemSizeT>(output.size() - (startSize + sizeof(ElemSizeT)));
        const ElemSizeT leWrittenBytes = Utility::toBigEndian(writtenBytes);
        if (memcpy_s(output.data() + startSize, output.size() - writtenBytes, &leWrittenBytes, sizeof(ElemSizeT)) != 0)
            return false;

        return true;
    }

    inline bool IByteSerializable::writeSize(const ElemSizeT size, std::vector<char>& output)
    {
        const size_t startSize = output.size();
        output.resize(startSize + sizeof(ElemSizeT));

        const ElemSizeT leSize = Utility::toBigEndian(size);
        if (memcpy_s(output.data() + startSize, output.size() - startSize, &leSize, sizeof(ElemSizeT)) != 0)
            return false;

        return true;
    }

    inline IByteSerializable::ElemSizeT IByteSerializable::readSize(const void* data, const size_t length)
    {
        if (data == nullptr || length < sizeof(ElemSizeT))
            return INVALID_ELEMENT_SIZE;

        ElemSizeT elemSize = 0;

        if (memcpy_s(&elemSize, sizeof(ElemSizeT), data, sizeof(ElemSizeT)) != 0)
            return INVALID_ELEMENT_SIZE;

        return Utility::fromBigEndian(elemSize);
    }

    template <typename SizeT>
    bool IByteSerializable::serializeString(const std::string& string, std::vector<char>& output)
    {
        const SizeT strLength = static_cast<SizeT>(string.size());

        size_t offset = output.size();
        output.resize(offset + sizeof(SizeT) + strLength, 0);

        const SizeT leStrLength = Utility::toBigEndian(strLength);
        if (memcpy_s(output.data() + offset, output.size() - offset, &leStrLength, sizeof(SizeT)) != 0)
            return false;

        offset += sizeof(SizeT);
        if (memcpy_s(output.data() + offset, output.size() - offset, string.c_str(), strLength) != 0)
            return false;

        return true;
    }

    template <typename SizeT>
    bool IByteSerializable::deserializeString(std::string& out, const char* data, const size_t length)
    {
        if (data == nullptr || length < sizeof(SizeT))
            return false;

        SizeT strLength = 0;

        if (memcpy_s(&strLength, sizeof(SizeT), data, sizeof(SizeT)) != 0)
            return false;

        strLength = Utility::fromBigEndian(strLength);

        const size_t offset = sizeof(SizeT);
        out.resize(strLength);

        if (length <= offset || length - offset < strLength || memcpy_s(out.data(), out.size(), data + offset, strLength) != 0)
            return false;

        return true;
    }

#ifdef __LIBMATH__VECTOR__VECTOR3_H__
    inline bool serializeVector3(LibMath::Vector3 vec3, std::vector<char>& output)
    {
        vec3.m_x = Utility::toBigEndian(vec3.m_x);
        vec3.m_y = Utility::toBigEndian(vec3.m_y);
        vec3.m_z = Utility::toBigEndian(vec3.m_z);

        const size_t startSize = output.size();
        output.resize(startSize + sizeof(LibMath::Vector3));
        return memcpy_s(output.data() + startSize, output.size() - startSize, vec3.getArray(), sizeof(LibMath::Vector3)) == 0;
    }

    inline bool deserializeVector3(LibMath::Vector3& out, const char* data, const size_t length)
    {
        if (data == nullptr || length < sizeof(LibMath::Vector3))
            return false;

        if (memcpy_s(out.getArray(), sizeof(LibMath::Vector3), data, sizeof(LibMath::Vector3)) != 0)
            return false;

        out.m_x = Utility::fromBigEndian(out.m_x);
        out.m_y = Utility::fromBigEndian(out.m_y);
        out.m_z = Utility::fromBigEndian(out.m_z);

        return true;
    }
#endif
}
