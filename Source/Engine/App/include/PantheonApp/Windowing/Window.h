#pragma once
#include "PantheonApp/Core/IContext.h"
#include "PantheonApp/Input/EInputModifier.h"
#include "PantheonApp/Input/EKey.h"
#include "PantheonApp/Input/EKeyState.h"
#include "PantheonApp/Input/EMouseButton.h"
#include "PantheonApp/Input/EMouseButtonState.h"
#include "PantheonApp/Windowing/WindowSettings.h"

#include <PantheonCore/Eventing/Event.h>

#include <Vector/Vector2.h>

// Forward declaration of GLFWwindow to avoid including glfw in the header
struct GLFWwindow;

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
        ~Window() = default;

        /**
         * \brief Disable the Application's copy assignment operator
         */
        Window& operator=(const Window&) = delete;

        /**
         * \brief Disable the Application's move assignment operator
         */
        Window& operator=(Window&&) = delete;

        /**
         * \brief Sets the window as glfw's current context
         */
        void makeCurrentContext() const;

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
         * \brief Swaps the render buffers
         */
        void swapBuffers();

        /**
         * \brief Sets whether the window should close or not
         * \param shouldClose Whether the window should close or not
         */
        void setShouldClose(bool shouldClose) const;

        /**
         * \brief Gets the window's aspect ratio
         * \return The window's aspect ratio
         */
        float getAspect() const;

        /**
         * \brief Gets the window's refresh rate
         * \return The window's refresh rate
         */
        int getRefreshRate() const;

        /**
         * \brief Sets the window's refresh rate
         * \param refreshRate The window's new refresh rate
         */
        void setRefreshRate(int refreshRate);

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

    private:
        inline static std::unordered_map<void*, Window*> s_windowsMap;

        std::string m_title;
        DimensionsT m_size;
        DimensionsT m_minSize;
        DimensionsT m_maxSize;
        PosT        m_pos;

        Core::IContext* m_context;
        void*           m_handle;

        int  m_refreshRate;
        bool m_isFullScreen;

        /**
         * \brief Creates a glfw window with the given settings
         * \param settings The window creation settings
         */
        void createHandle(const WindowSettings& settings);

        /**
         * \brief Binds the glfw callbacks
         */
        void bindCallbacks() const;

        /**
         * \brief Updates the glfw window's size limits based
         */
        void updateSizeLimits() const;

        /**
         * \brief Finds the window linked to the given handle
         * \param window The glfw window linked to the window to find.
         * \return A pointer to the found window or nullptr if no window was found.
         */
        static Window* getInstance(void* window);

        /**
         * \brief GLFW key action callback
         * \param glfwWindow The window for which a key action has been performed
         * \param key The key that has been interacted with
         * \param scanCode The scan code of the key that has been interacted with
         * \param action The action that has been performed
         * \param mods The input modifiers of the key event
         */
        static void onKey(GLFWwindow* glfwWindow, int key, int scanCode, int action, int mods);

        /**
         * \brief GLFW mouse button action callback
         * \param glfwWindow The window for which a mouse button has been interacted with
         * \param button The mouse button that has been interacted with
         * \param action The action that has been performed
         * \param mods The input modifiers of the key event
         */
        static void onMouseButton(GLFWwindow* glfwWindow, int button, int action, int mods);

        /**
         * \brief GLFW cursor position callback
         * \param glfwWindow The window in which the cursor moved
         * \param x The cursor's new x position
         * \param y The cursor's new y position
         */
        static void onCursorMove(GLFWwindow* glfwWindow, double x, double y);

        /**
         * \brief GLFW window move callback
         * \param glfwWindow The moved window
         * \param x The window's new x position
         * \param y The window's new y position
         */
        static void onMove(GLFWwindow* glfwWindow, int x, int y);

        /**
         * \brief GLFW window size change callback
         * \param glfwWindow The window for which the frame buffer size has changed
         * \param width The window's frame buffer's new width
         * \param height The window's frame buffer's new height
         */
        static void onResize(GLFWwindow* glfwWindow, int width, int height);

        /**
         * \brief GLFW frame buffer size change callback
         * \param glfwWindow The window for which the frame buffer size has changed
         * \param width The window's frame buffer's new width
         * \param height The window's frame buffer's new height
         */
        static void onFrameBufferResize(GLFWwindow* glfwWindow, int width, int height);

        /**
         * \brief GLFW window focus change callback
         * \param glfwWindow The window of which focus status changed
         * \param focused Whether the window is focused or not
         */
        static void onFocus(GLFWwindow* glfwWindow, int focused);

        /**
         * \brief GLFW window iconification callback
         * \param glfwWindow The window of which iconification status changed
         * \param iconified Whether the window is iconified or not
         */
        static void onIconify(GLFWwindow* glfwWindow, int iconified);

        /**
         * \brief GLFW window close callback
         * \param glfwWindow THe closed window
         */
        static void onClose(GLFWwindow* glfwWindow);
    };
}
