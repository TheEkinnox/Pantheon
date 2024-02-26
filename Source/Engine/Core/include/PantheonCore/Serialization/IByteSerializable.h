#pragma once
#include <Quaternion.h>
#include <Vector/Vector2.h>
#include <Vector/Vector3.h>
#include <Vector/Vector4.h>

#include <string>
#include <vector>

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
         * \return The number of deserialized bytes on success. 0 otherwise.
         */
        virtual size_t deserialize(const void* data, size_t length) = 0;

        /**
         * \brief Serializes the object to a byte array preceded by it's buffer size
         * \tparam SizeT The size's type
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        template <typename SizeT = ElemSizeT>
        bool serializeWithSize(std::vector<char>& output) const;

        /**
         * \brief Writes the given size to a byte array
         * \tparam SizeT The size's type
         * \param size The size to write
         * \param output The output memory buffer
         * \return True on success. False otherwise
         */
        template <typename SizeT = ElemSizeT>
        static bool writeNumber(SizeT size, std::vector<char>& output);

        /**
         * \brief Reads an element size from the given buffer
         * \note IMPORTANT: This function is guaranteed to work ONLY for values serialized using IByteSerializable::writeSize
         * or IByteSerializable::serializeWithSize and the SAME Size Type
         * \tparam SizeT The size's type
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return The deserialized size on success. INVALID_ELEMENT_SIZE otherwise
         */
        template <typename SizeT = ElemSizeT>
        static SizeT readNumber(const void* data, size_t length);

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
         * \return The number of deserialized bytes on success. 0 otherwise.
         */
        template <typename SizeT = size_t>
        static size_t deserializeString(std::string& out, const char* data, size_t length);

        /**
         * \brief Serializes the given 2d vector to a byte array
         * \param vec2 The vector to serialize
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        inline static bool serializeVector2(LibMath::Vector2 vec2, std::vector<char>& output);

        /**
         * \brief Deserializes the given 2d vector from a byte array
         * \param out The output vector
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return True on success. False otherwise.
         */
        inline static bool deserializeVector2(LibMath::Vector2& out, const char* data, size_t length);

        /**
         * \brief Converts the vector's members to big endian
         * \param vec2 The vector to convert to big endian
         */
        inline static void vec2ToBigEndian(LibMath::Vector2& vec2);

        /**
         * \brief Converts the vector's members from big endian to the system's byte order
         * \param vec2 The vector to convert from big endian
         */
        inline static void vec2FromBigEndian(LibMath::Vector2& vec2);

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

        /**
         * \brief Converts the vector's members to big endian
         * \param vec3 The vector to convert to big endian
         */
        inline static void vec3ToBigEndian(LibMath::Vector3& vec3);

        /**
         * \brief Converts the vector's members from big endian to the system's byte order
         * \param vec3 The vector to convert from big endian
         */
        inline static void vec3FromBigEndian(LibMath::Vector3& vec3);

        /**
         * \brief Serializes the given 4d vector to a byte array
         * \param vec4 The vector to serialize
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        inline static bool serializeVector4(LibMath::Vector4 vec4, std::vector<char>& output);

        /**
         * \brief Deserializes the given 4d vector from a byte array
         * \param out The output vector
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return True on success. False otherwise.
         */
        inline static bool deserializeVector4(LibMath::Vector4& out, const char* data, size_t length);

        /**
         * \brief Converts the vector's members to big endian
         * \param vec4 The vector to convert to big endian
         */
        inline static void vec4ToBigEndian(LibMath::Vector4& vec4);

        /**
         * \brief Converts the vector's members from big endian to the system's byte order
         * \param vec4 The vector to convert from big endian
         */
        inline static void vec4FromBigEndian(LibMath::Vector4& vec4);

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

        /**
         * \brief Serializes the given matrix to a byte array
         * \tparam Rows The number of rows in the given matrix
         * \tparam Cols The number of columns in the given matrix
         * \tparam DataT The data type of the given matrix
         * \param matrix The matrix to serialize
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        template <LibMath::length_t Rows, LibMath::length_t Cols, typename DataT>
        static bool serializeMatrix(LibMath::TMatrix<Rows, Cols, DataT> matrix, std::vector<char>& output);

        /**
         * \brief Deserializes the given matrix from a byte array
         * \tparam Rows The number of rows in the given matrix
         * \tparam Cols The number of columns in the given matrix
         * \tparam DataT The data type of the given matrix
         * \param out The output matrix
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return True on success. False otherwise.
         */
        template <LibMath::length_t Rows, LibMath::length_t Cols, typename DataT>
        static bool deserializeMatrix(LibMath::TMatrix<Rows, Cols, DataT>& out, const char* data, size_t length);
    };
}

#include "PantheonCore/Serialization/IByteSerializable.inl"
