#pragma once

namespace PantheonCore::Serialization
{
    class IByteSerializable
    {
    public:
        // Max element size is ~4GB (2^32-2 Bytes)
        using ElemSizeT = uint32_t;

        // Max element count is ~4GB (2^32-1 Bytes)
        using ElemCountT = uint32_t;

        static constexpr ElemSizeT INVALID_ELEMENT_SIZE = static_cast<ElemSizeT>(-1);

        virtual ~IByteSerializable() = default;

        /**
         * \brief Serializes the object to a byte array
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        virtual bool serialize(std::vector<char>& output) const = 0;

        /**
         * \brief Deserializes the object from the given memory buffer
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return True on success. False otherwise.
         */
        virtual bool deserialize(const void* data, size_t length) = 0;

        /**
         * \brief Serializes the object to a byte array preceded by it's buffer size
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        inline bool serializeWithSize(std::vector<char>& output) const;

        /**
         * \brief Writes the given size to a byte array
         * \param size The size to write
         * \param output The output memory buffer
         * \return True on success. False otherwise
         */
        inline static bool writeSize(ElemSizeT size, std::vector<char>& output);

        /**
         * \brief Reads an element size from the given buffer
         * \note IMPORTANT: This function is guaranteed to work ONLY for values serialized using IByteSerializable::writeSize
         * or IByteSerializable::serializeWithSize
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return The deserialized size on success. INVALID_ELEMENT_SIZE otherwise
         */
        inline static ElemSizeT readSize(const void* data, size_t length);

        /**
         * \brief Serializes the given string to a byte array
         * \tparam SizeT The string's size type
         * \param string The string to serialize
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        template <typename SizeT = size_t>
        static bool serializeString(const std::string& string, std::vector<char>& output);

        /**
         * \brief Deserializes the given string from a byte array
         * \note On success the read bytes will always be sizeof(SizeT) + out.size()
         * \tparam SizeT The string's size type
         * \param out The output string
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return True on success. False otherwise.
         */
        template <typename SizeT = size_t>
        static bool deserializeString(std::string& out, const char* data, size_t length);

#ifdef __LIBMATH__VECTOR__VECTOR3_H__
        /**
         * \brief Serializes the given 3d vector to a byte array
         * \param vec3 The vector to serialize
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        inline static bool serializeVector3(LibMath::Vector3 vec3, std::vector<char>& output);

        /**
         * \brief Deserializes the given 3d vector from a byte array
         * \param out The output vector
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return True on success. False otherwise.
         */
        inline static bool deserializeVector3(LibMath::Vector3& out, const char* data, size_t length);
#endif

#ifdef __LIBMATH__QUATERNION_H__
        /**
         * \brief Serializes the given quaternion to a byte array
         * \param quat The quaternion to serialize
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        inline static bool serializeQuaternion(LibMath::Quaternion quat, std::vector<char>& output);

        /**
         * \brief Deserializes the given quaternion from a byte array
         * \param out The output quaternion
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return True on success. False otherwise.
         */
        inline static bool deserializeQuaternion(LibMath::Quaternion& out, const char* data, size_t length);
#endif
    };
}

#include "PantheonCore/Serialization/IByteSerializable.inl"
