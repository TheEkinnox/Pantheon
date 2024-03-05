#pragma once
#include <cstdint>
#include <memory>
#include <vector>

namespace PantheonRendering::RHI
{
    class IIndexBuffer
    {
    public:
        IIndexBuffer(const IIndexBuffer&)           = delete;
        IIndexBuffer(IIndexBuffer&& other) noexcept = default;

        virtual ~IIndexBuffer() = default;

        IIndexBuffer& operator=(const IIndexBuffer&)           = delete;
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
        IIndexBuffer() = default;
    };
}
