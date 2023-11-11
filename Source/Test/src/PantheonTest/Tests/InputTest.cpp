#include "PantheonTest/Tests/InputTest.h"

#include <iostream>

#include <PantheonApp/Input/EKey.h>
#include <PantheonApp/Input/InputManager.h>

#include <PantheonCore/Debug/Logger.h>
#include <PantheonCore/Utility/ServiceLocator.h>

using namespace PantheonEngine::Application::Input;
using namespace PantheonEngine::Application::Windowing;

namespace PantheonTest
{
    InputTest::InputTest()
        : InputTest("Input")
    {
    }

    InputTest::InputTest(const std::string& name)
        : ITest(name)
    {
    }

    void InputTest::onUpdate()
    {
        testKeyboard();
        testMouse();
    }

    void InputTest::onPostUpdate()
    {
        if (m_frameCount++ == 3)
            complete();
    }

    void InputTest::testKeyboard() const
    {
        const auto& inputManager = PTH_SERVICE(InputManager);
        const auto& window = PTH_SERVICE(Window);

        switch (m_frameCount)
        {
        case 0:
        {
            // Keyboard
            window.m_keyEvent.invoke(EKey::P, 0, EKeyState::PRESSED, EInputModifier::NONE);

            ASSERT(inputManager.isKeyPressed(EKey::P));
            ASSERT(inputManager.isKeyDown(EKey::P));
            ASSERT(!inputManager.isKeyReleased(EKey::P));
            ASSERT(!inputManager.isKeyUp(EKey::P));
            break;
        }
        case 1:
        {
            ASSERT(!inputManager.isKeyPressed(EKey::P));
            ASSERT(inputManager.isKeyDown(EKey::P));
            ASSERT(!inputManager.isKeyReleased(EKey::P));
            ASSERT(!inputManager.isKeyUp(EKey::P));

            window.m_keyEvent.invoke(EKey::P, 0, EKeyState::REPEATED, EInputModifier::NONE);

            ASSERT(!inputManager.isKeyPressed(EKey::P));
            ASSERT(inputManager.isKeyDown(EKey::P));
            ASSERT(!inputManager.isKeyReleased(EKey::P));
            ASSERT(!inputManager.isKeyUp(EKey::P));
            break;
        }
        case 2:
        {
            ASSERT(!inputManager.isKeyPressed(EKey::P));
            ASSERT(inputManager.isKeyDown(EKey::P));
            ASSERT(!inputManager.isKeyReleased(EKey::P));
            ASSERT(!inputManager.isKeyUp(EKey::P));

            window.m_keyEvent.invoke(EKey::P, 0, EKeyState::RELEASED, EInputModifier::NONE);

            ASSERT(!inputManager.isKeyPressed(EKey::P));
            ASSERT(!inputManager.isKeyDown(EKey::P));
            ASSERT(inputManager.isKeyReleased(EKey::P));
            ASSERT(inputManager.isKeyUp(EKey::P));
            break;
        }
        case 3:
        {
            ASSERT(!inputManager.isKeyPressed(EKey::P));
            ASSERT(!inputManager.isKeyDown(EKey::P));
            ASSERT(!inputManager.isKeyReleased(EKey::P));
            ASSERT(inputManager.isKeyUp(EKey::P));
            break;
        }
        default:
            break;
        }
    }

    void InputTest::testMouse() const
    {
        const auto& inputManager = PTH_SERVICE(InputManager);
        const auto& window = PTH_SERVICE(Window);

        switch (m_frameCount)
        {
        case 0:
        {
            window.m_mouseButtonEvent.invoke(EMouseButton::LEFT, EMouseButtonState::PRESSED, EInputModifier::NONE);

            ASSERT(inputManager.isMouseButtonPressed(EMouseButton::LEFT));
            ASSERT(inputManager.isMouseButtonDown(EMouseButton::LEFT));
            ASSERT(!inputManager.isMouseButtonReleased(EMouseButton::LEFT));
            ASSERT(!inputManager.isMouseButtonUp(EMouseButton::LEFT));
            break;
        }
        case 1:
        {
            ASSERT(!inputManager.isMouseButtonPressed(EMouseButton::LEFT));
            ASSERT(inputManager.isMouseButtonDown(EMouseButton::LEFT));
            ASSERT(!inputManager.isMouseButtonReleased(EMouseButton::LEFT));
            ASSERT(!inputManager.isMouseButtonUp(EMouseButton::LEFT));

            window.m_mouseButtonEvent.invoke(EMouseButton::LEFT, EMouseButtonState::RELEASED, EInputModifier::NONE);

            ASSERT(!inputManager.isMouseButtonPressed(EMouseButton::LEFT));
            ASSERT(!inputManager.isMouseButtonDown(EMouseButton::LEFT));
            ASSERT(inputManager.isMouseButtonReleased(EMouseButton::LEFT));
            ASSERT(inputManager.isMouseButtonUp(EMouseButton::LEFT));
            break;
        }
        case 2:
        {
            ASSERT(!inputManager.isMouseButtonPressed(EMouseButton::LEFT));
            ASSERT(!inputManager.isMouseButtonDown(EMouseButton::LEFT));
            ASSERT(!inputManager.isMouseButtonReleased(EMouseButton::LEFT));
            ASSERT(inputManager.isMouseButtonUp(EMouseButton::LEFT));
            break;
        }
        default:
            break;
        }
    }
}
