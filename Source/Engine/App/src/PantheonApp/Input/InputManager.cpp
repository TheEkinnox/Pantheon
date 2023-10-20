#include "PantheonApp/Input/InputManager.h"

#include <GLFW/glfw3.h>
#include <functional>

#include "PantheonApp/Input/EKeyState.h"
#include "PantheonApp/Windowing/Window.h"

#include "PantheonCore/Utility/ServiceLocator.h"
#include "PantheonCore/Utility/macros.h"

using namespace PantheonEngine::Application::Windowing::Exceptions;
using namespace PantheonEngine::Application::Windowing;
using namespace PantheonEngine::Core::Utility;

namespace PantheonEngine::Application::Input
{
#pragma region BINDINGS
    extern "C"
    {
        PANTHEON_API void InputManager_getMousePosition(double& x, double& y)
        {
            const auto [mouseX, mouseY] = PTH_SERVICE(InputManager).getMousePosition();
            x = mouseX;
            y = mouseY;
        }

        PANTHEON_API void InputManager_getMouseDelta(double& x, double& y)
        {
            const auto [mouseX, mouseY] = PTH_SERVICE(InputManager).getMousePosition();
            x = mouseX;
            y = mouseY;
        }

        PANTHEON_API int InputManager_getScanCode(const int key)
        {
            return InputManager::getScanCode(static_cast<EKey>(key));
        }

        PANTHEON_API const char* InputManager_getKeyName(const int key, const int scanCode)
        {
            return InputManager::getKeyName(static_cast<EKey>(key), scanCode);
        }

        EXPORT_SERVICE_FUNC(/**/, /**/, InputManager, bool, isKeyUp, int key, static_cast<EKey>(key))
        EXPORT_SERVICE_FUNC(/**/, /**/, InputManager, bool, isKeyDown, int key, static_cast<EKey>(key))
        EXPORT_SERVICE_FUNC(/**/, /**/, InputManager, bool, isKeyPressed, int key, static_cast<EKey>(key))
        EXPORT_SERVICE_FUNC(/**/, /**/, InputManager, bool, isKeyReleased, int key, static_cast<EKey>(key))

        EXPORT_SERVICE_FUNC(/**/, ScanCode, InputManager, bool, isKeyUp, int scanCode, scanCode)
        EXPORT_SERVICE_FUNC(/**/, ScanCode, InputManager, bool, isKeyDown, int scanCode, scanCode)
        EXPORT_SERVICE_FUNC(/**/, ScanCode, InputManager, bool, isKeyPressed, int scanCode, scanCode)
        EXPORT_SERVICE_FUNC(/**/, ScanCode, InputManager, bool, isKeyReleased, int scanCode, scanCode)

        EXPORT_SERVICE_FUNC(/**/, /**/, InputManager, bool, isMouseButtonUp, int mouseButton,
            static_cast<EMouseButton>(mouseButton))
        EXPORT_SERVICE_FUNC(/**/, /**/, InputManager, bool, isMouseButtonDown, int mouseButton,
            static_cast<EMouseButton>(mouseButton))
        EXPORT_SERVICE_FUNC(/**/, /**/, InputManager, bool, isMouseButtonPressed, int mouseButton,
            static_cast<EMouseButton>(mouseButton))
        EXPORT_SERVICE_FUNC(/**/, /**/, InputManager, bool, isMouseButtonReleased, int mouseButton,
            static_cast<EMouseButton>(mouseButton))
    }
#pragma endregion

    InputManager::InputManager(Window& window) :
        m_window(window), m_isFirstMouse(true)
    {
        using namespace std::placeholders;

        m_keyCallbackId = m_window.m_keyEvent.subscribe(std::bind(&InputManager::keyCallback, this, _1, _2, _3, _4));
        m_mouseButtonCallbackId = m_window.m_mouseButtonEvent.subscribe(std::bind(&InputManager::mouseButtonCallback,
            this, _1, _2, _3));
    }

    InputManager::~InputManager()
    {
        m_window.m_keyEvent.unsubscribe(m_keyCallbackId);
        m_window.m_mouseButtonEvent.unsubscribe(m_mouseButtonCallbackId);
    }

    void InputManager::update()
    {
        m_mousePos = m_window.getCursorPosition();

        if (m_isFirstMouse)
        {
            m_lastMousePos = m_mousePos;
            m_isFirstMouse = false;
        }

        m_mouseDelta.m_x = m_mousePos.m_x - m_lastMousePos.m_x;
        m_mouseDelta.m_y = m_mousePos.m_y - m_lastMousePos.m_y;

        m_lastMousePos = m_mousePos;
        ++m_currentFrame;
    }

    void InputManager::resetFirstMouse()
    {
        m_isFirstMouse = false;
    }

    Window::CursorPosT InputManager::getMousePosition() const
    {
        return m_mousePos;
    }

    Window::CursorPosT InputManager::getMouseDelta() const
    {
        return m_mouseDelta;
    }

    int InputManager::getScanCode(const EKey key)
    {
        return glfwGetKeyScancode(static_cast<int>(key));
    }

    const char* InputManager::getKeyName(const EKey key, const int scanCode)
    {
        return glfwGetKeyName(static_cast<int>(key), scanCode);
    }

    bool InputManager::isKeyUp(const EKey key) const
    {
        return !m_keyInfos.contains(key)
            || m_keyInfos.at(key).m_keyState == EKeyState::KEY_RELEASED;
    }

    bool InputManager::isKeyUp(const int scanCode) const
    {
        return !m_scanCodeInfo.contains(scanCode)
            || m_scanCodeInfo.at(scanCode).m_keyState == EKeyState::KEY_RELEASED;
    }

    bool InputManager::isKeyDown(const EKey key) const
    {
        return m_keyInfos.contains(key)
            && (m_keyInfos.at(key).m_keyState == EKeyState::KEY_PRESSED
                || m_keyInfos.at(key).m_keyState == EKeyState::KEY_REPEATED);
    }

    bool InputManager::isKeyDown(const int scanCode) const
    {
        return m_scanCodeInfo.contains(scanCode)
            && (m_scanCodeInfo.at(scanCode).m_keyState == EKeyState::KEY_PRESSED
                || m_scanCodeInfo.at(scanCode).m_keyState == EKeyState::KEY_REPEATED);
    }

    bool InputManager::isKeyPressed(const EKey key) const
    {
        return m_keyInfos.contains(key)
            && m_keyInfos.at(key).m_keyState == EKeyState::KEY_PRESSED
            && m_keyInfos.at(key).m_stateChangeFrame == m_currentFrame;
    }

    bool InputManager::isKeyPressed(const int scanCode) const
    {
        return m_scanCodeInfo.contains(scanCode)
            && m_scanCodeInfo.at(scanCode).m_keyState == EKeyState::KEY_PRESSED
            && m_scanCodeInfo.at(scanCode).m_stateChangeFrame == m_currentFrame;
    }

    bool InputManager::isKeyReleased(const EKey key) const
    {
        return m_keyInfos.contains(key)
            && m_keyInfos.at(key).m_keyState == EKeyState::KEY_RELEASED
            && m_keyInfos.at(key).m_stateChangeFrame == m_currentFrame;
    }

    bool InputManager::isKeyReleased(const int scanCode) const
    {
        return m_scanCodeInfo.contains(scanCode)
            && m_scanCodeInfo.at(scanCode).m_keyState == EKeyState::KEY_RELEASED
            && m_scanCodeInfo.at(scanCode).m_stateChangeFrame == m_currentFrame;
    }

    bool InputManager::isMouseButtonUp(const EMouseButton button) const
    {
        return !m_mouseButtonInfo.contains(button)
            || m_mouseButtonInfo.at(button).m_buttonState == EMouseButtonState::MOUSE_RELEASED;
    }

    bool InputManager::isMouseButtonDown(const EMouseButton button) const
    {
        return m_mouseButtonInfo.contains(button)
            && m_mouseButtonInfo.at(button).m_buttonState == EMouseButtonState::MOUSE_PRESSED;
    }

    bool InputManager::isMouseButtonPressed(const EMouseButton button) const
    {
        return m_mouseButtonInfo.contains(button)
            && m_mouseButtonInfo.at(button).m_buttonState == EMouseButtonState::MOUSE_PRESSED
            && m_mouseButtonInfo.at(button).m_stateChangeFrame == m_currentFrame;
    }

    bool InputManager::isMouseButtonReleased(const EMouseButton button) const
    {
        return m_mouseButtonInfo.contains(button)
            && m_mouseButtonInfo.at(button).m_buttonState == EMouseButtonState::MOUSE_RELEASED
            && m_mouseButtonInfo.at(button).m_stateChangeFrame == m_currentFrame;
    }

    void InputManager::clearStates()
    {
        m_keyInfos.clear();
        m_scanCodeInfo.clear();
        m_mouseButtonInfo.clear();
    }

    void InputManager::keyCallback(const EKey key, const int scanCode, const EKeyState state, EInputModifier)
    {
        m_keyInfos[key] = { state, m_currentFrame };
        m_scanCodeInfo[scanCode] = { state, m_currentFrame };
    }

    void InputManager::mouseButtonCallback(const EMouseButton button, const EMouseButtonState state, EInputModifier)
    {
        m_mouseButtonInfo[button] = { state, m_currentFrame };
    }
}
