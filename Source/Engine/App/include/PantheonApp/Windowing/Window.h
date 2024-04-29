#pragma once
#include "PantheonApp/Input/EInputModifier.h"
#include "PantheonApp/Input/EKey.h"
#include "PantheonApp/Input/EKeyState.h"
#include "PantheonApp/Input/EMouseButton.h"
#include "PantheonApp/Input/EMouseButtonState.h"
#include "PantheonApp/Windowing/WindowSettings.h"

#include <PantheonCore/Eventing/Event.h>

#include <Vector/Vector2.h>

namespace PantheonApp::Core
{
    class IContext;
}

namespace PantheonApp::Windowing
{
    class Window
    {
    public:
        using DimensionsT = LibMath::Vector2I;
        using PosT = LibMath::Vector2I;
        using CursorPosT = LibMath::TVector2<double>;

        PantheonCore::Eventing::Event<Input::EKey, int, Input::EKeyState, Input::EInputModifier>            m_keyEvent;
        PantheonCore::Eventing::Event<Input::EMouseButton, Input::EMouseButtonState, Input::EInputModifier> m_mouseButtonEvent;

        PantheonCore::Eventing::Event<DimensionsT> m_resizeEvent;
        PantheonCore::Eventing::Event<DimensionsT> m_framebufferResizeEvent;
        PantheonCore::Eventing::Event<PosT>        m_moveEvent;
        PantheonCore::Eventing::Event<CursorPosT>  m_cursorMoveEvent;
        PantheonCore::Eventing::Event<>            m_minimizeEvent;
        PantheonCore::Eventing::Event<>            m_maximizeEvent;
        PantheonCore::Eventing::Event<>            m_gainFocusEvent;
        PantheonCore::Eventing::Event<>            m_lostFocusEvent;
        PantheonCore::Eventing::Event<>            m_closeEvent;

        /**
         * \brief Creates a window
         * \param context The application's context
         * \param settings The window'S settings
         */
        Window(Core::IContext& context, const WindowSettings& settings);

        /**
         * \brief Disable the Application's copy constructor
         */
        Window(const Window&) = delete;

        /**
         * \brief Disable the Application's move constructor
         */
        Window(Window&&) = delete;

        /**
         * \brief Clears the application's previously allocated resources
         */
        ~Window();

        /**
         * \brief Disable the Application's copy assignment operator
         */
        Window& operator=(const Window&) = delete;

        /**
         * \brief Disable the Application's move assignment operator
         */
        Window& operator=(Window&&) = delete;

        /**
         * \brief Sets the window as it's context's main window
         */
        void makeMain() const;

        /**
         * \brief Gets the window's context
         * \return The window's context
         */
        Core::IContext& getContext() const;

        /**
         * \brief Gets the window's title
         * \return The window's title
         */
        std::string getTitle() const;

        /**
         * \brief Sets the window's title
         * \return The window's title
         */
        void setTitle(const std::string& title);

        /**
         * \brief Gets the window's current position
         * \return The window's current position
         */
        PosT getPosition() const;

        /**
         * \brief Sets the window's position
         */
        void setPosition(PosT pos);

        /**
         * \brief Gets the window's current size
         * \return The window's current size
         */
        DimensionsT getSize() const;

        /**
         * \brief Gets the window's aspect ratio
         * \return The window's aspect ratio
         */
        float getAspect() const;

        /**
         * \brief Sets the window's size
         */
        void setSize(DimensionsT size);

        /**
         * \brief Gets the window's minimum size
         * \return The window's minimum size
         */
        DimensionsT getMinSize() const;

        /**
         * \brief Sets the window's minimum size
         */
        void setMinSize(DimensionsT size);

        /**
         * \brief Gets the window's maximum size
         * \return The window's maximum size
         */
        DimensionsT getMaxSize() const;

        /**
         * \brief Sets the window's maximum size
         */
        void setMaxSize(DimensionsT size);

        /**
         * \brief Gets the current cursor position
         * \return The current cursor position
         */
        CursorPosT getCursorPosition() const;

        /**
         * \brief Sets the cursor's position
         * \param cursorPos The new cursor position
         */
        void setCursorPosition(CursorPosT cursorPos) const;

        /**
         * \brief Shows the mouse cursor
         */
        void showCursor() const;

        /**
         * \brief Hides the mouse cursor
         */
        void hideCursor() const;

        /**
         * \brief Hides the cursor and locks it to the window
         */
        void disableCursor() const;

        /**
         * \brief Whether the window should close or not
         * \return True if the window should close. False otherwise.
         */
        bool shouldClose() const;

        /**
         * \brief Sets whether the window should close or not
         * \param shouldClose Whether the window should close or not
         */
        void setShouldClose(bool shouldClose) const;

        /**
         * \brief Gets the window's refresh rate
         * \return The window's refresh rate
         */
        bool isFullScreen() const;

        /**
         * \brief Sets the window's fullscreen mode
         * \param shouldEnable Whether the fullscreen mode should be enabled or not
         */
        void setFullScreen(bool shouldEnable);

        /**
         * \brief Toggles the window's fullscreen mode
         */
        void toggleFullScreen();

        /**
         * \brief Swaps the render buffers
         */
        void swapBuffers();

    private:
        std::string m_title;
        DimensionsT m_minSize;
        DimensionsT m_maxSize;

        Core::IContext* m_context;
        void*           m_handle;

        bool m_isFullScreen;

        /**
         * \brief Updates the glfw window's size limits based
         */
        void updateSizeLimits() const;
    };
}
