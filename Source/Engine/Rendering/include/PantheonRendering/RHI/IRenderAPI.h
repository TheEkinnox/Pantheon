#pragma once
#include "PantheonRendering/Core/Color.h"
#include "PantheonRendering/Enums/EBlendFactor.h"
#include "PantheonRendering/Enums/ECompareAlgorithm.h"
#include "PantheonRendering/Enums/ECullFace.h"
#include "PantheonRendering/RHI/EGraphicsAPI.h"
#include "PantheonRendering/Enums/EPixelDataFormat.h"
#include "PantheonRendering/Enums/EPixelDataType.h"
#include "PantheonRendering/Enums/EPrimitiveType.h"
#include "PantheonRendering/Enums/ERenderingCapability.h"

#include <Vector/Vector2.h>

#include <string>

namespace PantheonRendering::RHI
{
    using PosT = LibMath::Vector2I;
    using DimensionsT = LibMath::Vector2I;

    class IRenderAPI
    {
    public:
        virtual ~IRenderAPI() = default;

        /**
         * \brief Initializes the graphics API
         * \param enableDebug Whether debugging should be enabled or not
         * \return A reference to the current API
         */
        virtual IRenderAPI& init(bool enableDebug) = 0;

        /**
         * \brief Clears the selected buffers
         * \param clearColor Whether the color buffer should be cleared
         * \param clearDepth Whether the depth buffer should be cleared
         * \param clearStencil Whether the stencil buffer should be cleared
         * \return A reference to the current API
         */
        virtual IRenderAPI& clear(bool clearColor, bool clearDepth, bool clearStencil) = 0;

        /**
         * \brief Reads pixels into the given buffer
         * \param position The position of the pixels to read
         * \param size The size of the area to read
         * \param dataFormat The format in which the pixels should be read
         * \param dataType The data type of the pixels to read
         * \param out The buffer in which to read pixels
         * \return A reference to the current API
         */
        virtual IRenderAPI& readPixels(const PosT&           position, const DimensionsT& size, Enums::EPixelDataFormat dataFormat,
                                       Enums::EPixelDataType dataType, void*              out) = 0;

        /**
         * \brief Draws the given number of indices
         * \param primitiveType The target primitive draw mode
         * \param count The number of elements to draw
         * \return A reference to the current API
         */
        virtual IRenderAPI& drawElements(Enums::EPrimitiveType primitiveType, uint32_t count) = 0;

        /**
         * \brief Draws the given number of instances of a model with the given number of indices
         * \param primitiveType The target primitive draw mode
         * \param count The number of elements to draw
         * \param instances The number of instances to draw
         * \return A reference to the current API
         */
        virtual IRenderAPI& drawElementsInstanced(Enums::EPrimitiveType primitiveType, uint32_t count, uint32_t instances) = 0;

        /**
         * \brief Draws the given number of vertices
         * \param primitiveType The target primitive draw mode
         * \param count The number of vertices to draw
         * \return A reference to the current API
         */
        virtual IRenderAPI& drawArrays(Enums::EPrimitiveType primitiveType, uint32_t count) = 0;

        /**
         * \brief Draws the given number of instances of a model with the given number of vertices
         * \param primitiveType The target primitive draw mode
         * \param count The number of vertices to draw
         * \param instances The number of instances to draw
         * \return A reference to the current API
         */
        virtual IRenderAPI& drawArraysInstanced(Enums::EPrimitiveType primitiveType, uint32_t count, uint32_t instances) = 0;

        /**
         * \brief Sets the state of the given rendering capability
         * \param capability The rendering capability of which state should be changed
         * \param enable Whether the rendering capability should be enabled
         * \return A reference to the current API
         */
        virtual IRenderAPI& setCapability(Enums::ERenderingCapability capability, bool enable) = 0;

        /**
         * \brief Checks whether a rendering capability is enabled or not
         * \param capability The rendering capability to check
         * \return True if the target rendering capability is enabled. False otherwise
         */
        virtual bool hasCapability(Enums::ERenderingCapability capability) = 0;

        /**
         * \brief Sets the sorting algorithm for depth testing
         * \param algorithm The new depth algorithm
         * \return A reference to the current API
         */
        virtual IRenderAPI& setDepthAlgorithm(Enums::ECompareAlgorithm algorithm) = 0;

        /**
         * \brief Sets the sorting algorithm and reference value for stencil testing
         * \param algorithm The new stencil test algorithm
         * \param reference The reference value
         * \param mask The stencil mask
         * \return A reference to the current API
         */
        virtual IRenderAPI& setStencilAlgorithm(Enums::ECompareAlgorithm algorithm, int32_t reference, uint32_t mask) = 0;

        /**
         * \brief Sets the clear color
         * \param color The new clear color
         * \return A reference to the current API
         */
        virtual IRenderAPI& setClearColor(const Core::Color& color) = 0;

        /**
         * \brief Gets the current clear color
         * \return The background clear color
         */
        virtual Core::Color getClearColor() = 0;

        /**
         * \brief Sets which face(s) should be culled
         * \param cullFace The new cull face
         * \return A reference to the current API
         */
        virtual IRenderAPI& setCullFace(Enums::ECullFace cullFace) = 0;

        /**
         * \brief Enables/disables depth writing
         * \param enable Whether depth writing should be enabled or not
         * \return A reference to the current API
         */
        virtual IRenderAPI& setDepthWriting(bool enable) = 0;

        /**
         * \brief Sets whether writing in the different color channels should be allowed or not
         * \param enableRed Whether writing in the red color channel should be allowed
         * \param enableGreen Whether writing in the green color channel should be allowed
         * \param enableBlue Whether writing in the blue color channel should be allowed
         * \param enableAlpha Whether writing in the alpha color channel should be allowed
         * \return A reference to the current API
         */
        virtual IRenderAPI& setColorWriting(bool enableRed, bool enableGreen, bool enableBlue, bool enableAlpha) = 0;

        /**
         * \brief Sets the source and destination blending factors
         * \param sourceFactor The blending factor to apply to the source color
         * \param destinationFactor The blending factor to apply to the destination color
         * \return A reference to the current API
         */
        virtual IRenderAPI& setBlendFunc(Enums::EBlendFactor sourceFactor, Enums::EBlendFactor destinationFactor) = 0;

        /**
         * \brief Sets the viewport's position and size
         * \param position The new viewport's position
         * \param size The new viewport's size
         * \return A reference to the current API
         */
        virtual IRenderAPI& setViewport(PosT position, DimensionsT size) = 0;

        /**
         * \brief Gets the used graphics API
         * \return The current graphics API
         */
        virtual EGraphicsAPI getBackend() = 0;

        /**
         * \brief Gets the name of the vendor for the current API implementation
         * \return The name of the vendor for the current API implementation
         */
        virtual std::string getVendor() = 0;

        /**
         * \brief Gets the current hardware's information
         * \return The current hardware string
         */
        virtual std::string getHardware() = 0;

        /**
         * \brief Gets the rendering API's current version
         * \return The API's current version
         */
        virtual std::string getVersion() = 0;

        /**
         * \brief Gets the rendering API's shading language version
         * \return The shading language version
         */
        virtual std::string getShadingLanguageVersion() = 0;

        /**
         * \brief Gets the current render api
         * \return A reference to the current render api
         */
        inline static IRenderAPI& getCurrent();

        /**
         * \brief Sets the current render api from the given backend
         * \param backend The target graphics backend
         * \return A reference to the current render api
         */
        static IRenderAPI& setCurrent(EGraphicsAPI backend);

    private:
        static std::unique_ptr<IRenderAPI> s_instance;
    };
}

#include "PantheonRendering/RHI/IRenderAPI.inl"
