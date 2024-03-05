#pragma once
#include "PantheonRendering/RHI/IVertexArray.h"

namespace PantheonRendering::RHI
{
    class NullVertexArray final : public IVertexArray
    {
    public:
        explicit NullVertexArray(const IVertexBuffer& vbo, const IIndexBuffer& ebo);

        NullVertexArray(const NullVertexArray&)     = delete;
        NullVertexArray(NullVertexArray&& other) noexcept = default;
        ~NullVertexArray() override                       = default;

        NullVertexArray& operator=(const NullVertexArray&)     = delete;
        NullVertexArray& operator=(NullVertexArray&& other) noexcept = default;

        /**
         * \brief Binds the vertex attributes object to the current context
         */
        void bind() const override;

        /**
         * \brief Unbinds the vertex attributes object from the current context
         */
        void unbind() const override;
    };
}
