#pragma once
#include <stdexcept>

#include "PantheonApp/Windowing/WindowSettings.h"

#include "PantheonApp/Core/Context.h"

#include "PantheonApp/Input/EInputModifier.h"
#include "PantheonApp/Input/EKey.h"
#include "PantheonApp/Input/EKeyState.h"
#include "PantheonApp/Input/EMouseButton.h"
#include "PantheonApp/Input/EMouseButtonState.h"

#include "PantheonCore/Eventing/Event.h"

// Forward declaration of GLFWwindow to avoid including glfw in the header
using GLFWwindow = struct GLFWwindow;

namespace PantheonEngine::Application::Windowing
{
    namespace Exceptions
    {
        class WindowCreationFailed : public std::runtime_error
        {
        public:
            WindowCreationFailed()
                : runtime_error("Window creation failed")
            {
            }

            explicit WindowCreationFailed(const std::string& message)
                : runtime_error(message.c_str())
            {
            }

            explicit WindowCreationFailed(const char* message)
                : runtime_error(message)
            {
            }
        };
    }

    class Window
    {
    public:
        using DimensionsT = std::pair<int, int>;
        using PosT = std::pair<int, int>;
        using CursorPosT = std::pair<double, double>;

        PantheonEngine::Core::Eventing::Event<Input::EKey, int, Input::EKeyState, Input::EInputModifier> m_keyEvent;
        PantheonEngine::Core::Eventing::Event<Input::EMouseButton, Input::EMouseButtonState, Input::EInputModifier> m_mouseButtonEvent;

        PantheonEngine::Core::Eventing::Event<DimensionsT> m_resizeEvent;
        PantheonEngine::Core::Eventing::Event<DimensionsT> m_framebufferResizeEvent;
        PantheonEngine::Core::Eventing::Event<PosT> m_moveEvent;
        PantheonEngine::Core::Eventing::Event<CursorPosT> m_cursorMoveEvent;
        PantheonEngine::Core::Eventing::Event<> m_minimizeEvent;
        PantheonEngine::Core::Eventing::Event<> m_maximizeEvent;
        PantheonEngine::Core::Eventing::Event<> m_gainFocusEvent;
        PantheonEngine::Core::Eventing::Event<> m_lostFocusEvent;
        PantheonEngine::Core::Eventing::Event<> m_closeEvent;

        /**
         * \brief Creates a GLFW window
         * \param context The application's context
         * \param settings The window'S settings
         */
        Window(const Core::Context& context, const WindowSettings& settings);

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
         * \brief Creates a glfw window with the given settings
         * \param settings The window creation settings
         */
        void createGlfwWindow(const WindowSettings& settings);

        /**
         * \brief Binds the glfw callbacks
         */
        void bindCallbacks() const;

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
        void swapBuffers() const;

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



    private:
        inline static std::unordered_map<GLFWwindow*, Window*> s_windowsMap;

        std::string m_title;
        DimensionsT m_size;
        DimensionsT m_minSize;
        DimensionsT m_maxSize;
        PosT m_pos;

        const Core::Context& m_context;
        GLFWwindow* m_glfwWindow;

        int m_refreshRate;
        bool m_isFullScreen;

        /**
         * \brief Updates the glfw window's size limits based
         */
        void updateSizeLimits() const;

        /**
         * \brief Finds the window linked to the given GLFW window
         * \param window The glfw window linked to the window to find.
         * \return A pointer to the found window or nullptr if no window was found.
         */
        static Window* getInstance(GLFWwindow* window);

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
