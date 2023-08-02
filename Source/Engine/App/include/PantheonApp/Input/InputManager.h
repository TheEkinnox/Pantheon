#pragma once
#include <unordered_map>

#include "PantheonApp/Input/EInputModifier.h"
#include "PantheonApp/Input/EKey.h"
#include "PantheonApp/Input/EKeyState.h"
#include "PantheonApp/Input/EMouseButton.h"
#include "PantheonApp/Input/EMouseButtonState.h"

#include "PantheonApp/Windowing/Window.h"

namespace PantheonEngine::Application::Input
{
    class InputManager
    {
    public:
        /**
         * \brief Creates an input manager instance linked to the given window
         * \param window A reference to the window linked to this input manager
         */
        explicit InputManager(Windowing::Window& window);

        /**
         * \brief Destroys the input manager
         */
        ~InputManager();

        /**
         * \brief Updates cached mouse data
         */
        void update();

        /**
         * \brief Resets the input manager to consider the mouse as not moved
         */
        void resetFirstMouse();

        /**
         * \brief Gets the cached mouse position
         * \return The cached mouse position
         */
        Windowing::Window::CursorPosT getMousePosition() const;

        /**
         * \brief Gets the delta between the current and last mouse positions
         * \return The difference between the current and previous mouse positions
         */
        Windowing::Window::CursorPosT getMouseDelta() const;

        /**
         * \brief Gets the layout dependent scan code associated with the given key
         * \param key The key we want the layout dependent scan code of
         * \return The layout dependent scan code of the given key
         */
        static int getScanCode(EKey key);

        /**
         * \brief Gets the name of the given key or scancode
         * \param key The key of which name to get (use KEY_UNKNOWN to use scanCode)
         * \param scanCode The scan code of which name to get (ignored if a valid key is provided)
         * \return The name of the given key or scancode
         */
        static const char* getKeyName(EKey key, int scanCode);

        /**
         * \brief Checks whether a given key is up
         * \param key The key to check
         * \return True if the key is up. False otherwise
         */
        bool isKeyUp(EKey key) const;

        /**
         * \brief Checks whether the key with a given scan code is up
         * \param scanCode The scan code of the key to check
         * \return True if the key is up. False otherwise.
         */
        bool isKeyUp(int scanCode) const;

        /**
         * \brief Checks whether a given key is down
         * \param key The key to check
         * \return True if the key is down. False otherwise.
         */
        bool isKeyDown(EKey key) const;

        /**
         * \brief Checks whether the key with a given scan code is down
         * \param scanCode The scan code of the key to check
         * \return True if the key is down. False otherwise.
         */
        bool isKeyDown(int scanCode) const;

        /**
         * \brief Checks whether a given key was pressed during the current frame
         * \param key The key to check
         * \return True if the key was pressed during the current frame. False otherwise.
         */
        bool isKeyPressed(EKey key) const;

        /**
         * \brief Checks whether a given key was pressed during the current frame
         * \param scanCode The scan code of the key to check
         * \return True if the key was pressed during the current frame. False otherwise.
         */
        bool isKeyPressed(int scanCode) const;

        /**
         * \brief Checks whether a given key was released during the current frame
         * \param key The key to check
         * \return True if the key was released during the current frame. False otherwise.
         */
        bool isKeyReleased(EKey key) const;

        /**
         * \brief Checks whether a given key was released during the current frame
         * \param scanCode The scan code of the key to check
         * \return True if the key was released during the current frame. False otherwise.
         */
        bool isKeyReleased(int scanCode) const;

        /**
         * \brief Checks whether a given mouse button is released
         * \param button The mouse button to check
         * \return True if the mouse button is released. False otherwise.
         */
        bool isMouseButtonUp(EMouseButton button) const;

        /**
         * \brief Checks whether a given mouse button is pressed
         * \param button The mouse button to check
         * \return True if the mouse button is down. False otherwise.
         */
        bool isMouseButtonDown(EMouseButton button) const;

        /**
         * \brief Checks whether a given mouse button was released during the current frame
         * \param button The mouse button to check
         * \return True if the mouse button was pressed during the current frame. False otherwise.
         */
        bool isMouseButtonPressed(EMouseButton button) const;

        /**
         * \brief Checks whether a given mouse button was released during the current frame
         * \param button The mouse button to check
         * \return True if the mouse button was released during the current frame. False otherwise.
         */
        bool isMouseButtonReleased(EMouseButton button) const;

        /**
         * \brief Clears the cached input states
         * \note Call this after each frame to make up and down functions
         * return true only if the action was performed in the same frame.
         */
        void clearStates();

    private:
        struct KeyInfo
        {
            EKeyState m_keyState;
            uint64_t m_stateChangeFrame;
        };

        struct MouseButtonInfo
        {
            EMouseButtonState m_buttonState;
            uint64_t m_stateChangeFrame;
        };

        Windowing::Window& m_window;

        Windowing::Window::CursorPosT m_mousePos;
        Windowing::Window::CursorPosT m_lastMousePos;
        Windowing::Window::CursorPosT m_mouseDelta;
        bool m_isFirstMouse;

        uint64_t m_keyCallbackId = 0;
        uint64_t m_mouseButtonCallbackId = 0;
        uint64_t m_currentFrame = 0;

        std::unordered_map<EKey, KeyInfo> m_keyInfos;
        std::unordered_map<int, KeyInfo> m_scanCodeInfo;
        std::unordered_map<EMouseButton, MouseButtonInfo> m_mouseButtonInfo;

        /**
         * \brief Window onKey callback - caches the given key's state
         * \param key The key that was interacted with
         * \param scanCode The key's scan code
         * \param state The new state of the key
         * \param mods The input modifiers of the key
         */
        void keyCallback(EKey key, int scanCode, EKeyState state, EInputModifier mods);

        /**
         * \brief Window onMouseButton callback - caches the given mouse button's state
         * \param button The mouse button that was interacted with
         * \param state The new state of the mouse button
         * \param mods The input modifiers of the mouse button
         */
        void mouseButtonCallback(EMouseButton button, EMouseButtonState state, EInputModifier mods);
    };
}
