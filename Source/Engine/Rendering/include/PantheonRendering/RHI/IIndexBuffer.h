#pragma once
#include <cstdint>
#include <memory>
#include <vector>

namespace PantheonRendering::RHI
{
    class IIndexBuffer
    {
    public:
        /**
         * \brief Disable index buffer copying
         */
        IIndexBuffer(const IIndexBuffer&) = delete;

        /**
         * \brief Creates a move copy of the given index buffer
         * \param other The moved index buffer
         */
        IIndexBuffer(IIndexBuffer&& other) noexcept = default;

        /**
         * \brief Destroys the given index buffer
         */
        virtual ~IIndexBuffer() = default;

        /**
         * \brief Disable index buffer copying
         */
        IIndexBuffer& operator=(const IIndexBuffer&) = delete;

        /**
         * \brief Moves the given index buffer into this one
         * \param other The moved index buffer
         * \return A reference to the modified index buffer
         */
        IIndexBuffer& operator=(IIndexBuffer&& other) noexcept = default;

        /**
         * \brief Binds the index buffer object to the current context
         */
        virtual void bind() const = 0;

        /**
         * \brief Unbinds the index buffer from the current context
         */
        virtual void unbind() const = 0;

        /**
         * \brief Creates a vertex buffer for the current render API with the given vertices
         * \param indices The array of indices to add to the buffer
         * \param count The number of indices in the buffer
         * \return The created index buffer
         */
        static std::unique_ptr<IIndexBuffer> create(const uint32_t* indices, uint32_t count);

        /**
         * \brief Creates a vertex buffer for the current render API with the given vertices
         * \param indices The array of indices to add to the buffer
         * \return The created index buffer
         */
        static std::unique_ptr<IIndexBuffer> create(const std::vector<uint32_t>& indices);

    protected:
        /**
         * \brief Creates a default index buffer
         */
        IIndexBuffer() = default;
    };
}
