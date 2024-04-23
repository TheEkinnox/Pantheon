#include "PantheonApp/Input/InputManager.h"

#include "PantheonApp/Input/EKeyState.h"
#include "PantheonApp/Windowing/Window.h"

#include <PantheonCore/Utility/ServiceLocator.h>

#include <functional>
#include <GLFW/glfw3.h>

using namespace PantheonApp::Windowing;
using namespace PantheonCore::Utility;

namespace PantheonApp::Input
{
    InputManager::InputManager(Window& window)
        : m_window(window), m_isFirstMouse(true)
    {
        const auto keyDelegate = [this](const EKey key, const int scanCode, const EKeyState state, const EInputModifier mods)
        {
            this->keyCallback(key, scanCode, state, mods);
        };

        const auto mouseDelegate = [this](const EMouseButton button, const EMouseButtonState state, const EInputModifier mods)
        {
            this->mouseButtonCallback(button, state, mods);
        };

        const auto focusGainedDelegate = [this]
        {
            this->resetFirstMouse();
            clearStates();
        };

        m_keyCallbackId         = m_window.m_keyEvent.subscribe(keyDelegate);
        m_mouseButtonCallbackId = m_window.m_mouseButtonEvent.subscribe(mouseDelegate);
        m_focusGainedCallbackId = m_window.m_gainFocusEvent.subscribe(focusGainedDelegate);
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
        m_isFirstMouse = true;
    }

    Window::CursorPosT InputManager::getMousePosition() const
    {
        return m_mousePos;
    }

    void InputManager::setMousePosition(const Window::CursorPosT position)
    {
        m_window.setCursorPosition(position);
        update();
        --m_currentFrame;
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

    EKeyState InputManager::getKeyState(const EKey key) const
    {
        const auto it = m_keyInfos.find(key);
        return it != m_keyInfos.end() ? it->second.m_keyState : EKeyState::RELEASED;
    }

    EKeyState InputManager::getScanCodeState(const int scanCode) const
    {
        const auto it = m_scanCodeInfo.find(scanCode);
        return it != m_scanCodeInfo.end() ? it->second.m_keyState : EKeyState::RELEASED;
    }

    EMouseButtonState InputManager::getMouseState(const EMouseButton button) const
    {
        const auto it = m_mouseButtonInfo.find(button);
        return it != m_mouseButtonInfo.end() ? it->second.m_buttonState : EMouseButtonState::RELEASED;
    }

    bool InputManager::isKeyUp(const EKey key) const
    {
        return getKeyState(key) == EKeyState::RELEASED;
    }

    bool InputManager::isKeyUp(const int scanCode) const
    {
        return getScanCodeState(scanCode) == EKeyState::RELEASED;
    }

    bool InputManager::isKeyDown(const EKey key) const
    {
        const EKeyState state = getKeyState(key);
        return state == EKeyState::PRESSED || state == EKeyState::REPEATED;
    }

    bool InputManager::isKeyDown(const int scanCode) const
    {
        const EKeyState state = getScanCodeState(scanCode);
        return state == EKeyState::PRESSED || state == EKeyState::REPEATED;
    }

    bool InputManager::isKeyPressed(const EKey key) const
    {
        const auto it = m_keyInfos.find(key);
        return it != m_keyInfos.end()
            && it->second.m_keyState == EKeyState::PRESSED
            && it->second.m_stateChangeFrame == m_currentFrame;
    }

    bool InputManager::isKeyPressed(const int scanCode) const
    {
        const auto it = m_scanCodeInfo.find(scanCode);
        return it != m_scanCodeInfo.end()
            && it->second.m_keyState == EKeyState::PRESSED
            && it->second.m_stateChangeFrame == m_currentFrame;
    }

    bool InputManager::isKeyReleased(const EKey key) const
    {
        const auto it = m_keyInfos.find(key);
        return it != m_keyInfos.end()
            && it->second.m_keyState == EKeyState::RELEASED
            && it->second.m_stateChangeFrame == m_currentFrame;
    }

    bool InputManager::isKeyReleased(const int scanCode) const
    {
        const auto it = m_scanCodeInfo.find(scanCode);
        return it != m_scanCodeInfo.end()
            && it->second.m_keyState == EKeyState::RELEASED
            && it->second.m_stateChangeFrame == m_currentFrame;
    }

    bool InputManager::isMouseButtonUp(const EMouseButton button) const
    {
        const EMouseButtonState state = getMouseState(button);
        return state == EMouseButtonState::RELEASED;
    }

    bool InputManager::isMouseButtonDown(const EMouseButton button) const
    {
        const EMouseButtonState state = getMouseState(button);
        return state == EMouseButtonState::PRESSED;
    }

    bool InputManager::isMouseButtonPressed(const EMouseButton button) const
    {
        const auto it = m_mouseButtonInfo.find(button);
        return it != m_mouseButtonInfo.end()
            && it->second.m_buttonState == EMouseButtonState::PRESSED
            && it->second.m_stateChangeFrame == m_currentFrame;
    }

    bool InputManager::isMouseButtonReleased(const EMouseButton button) const
    {
        const auto it = m_mouseButtonInfo.find(button);
        return it != m_mouseButtonInfo.end()
            && it->second.m_buttonState == EMouseButtonState::RELEASED
            && it->second.m_stateChangeFrame == m_currentFrame;
    }

    void InputManager::clearStates()
    {
        m_keyInfos.clear();
        m_scanCodeInfo.clear();
        m_mouseButtonInfo.clear();
    }

    void InputManager::keyCallback(const EKey key, const int scanCode, const EKeyState state, EInputModifier)
    {
        m_keyInfos[key]          = { state, m_currentFrame };
        m_scanCodeInfo[scanCode] = { state, m_currentFrame };
    }

    void InputManager::mouseButtonCallback(const EMouseButton button, const EMouseButtonState state, EInputModifier)
    {
        m_mouseButtonInfo[button] = { state, m_currentFrame };
    }
}
