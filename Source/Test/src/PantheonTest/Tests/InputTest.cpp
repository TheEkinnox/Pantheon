#include "PantheonTest/Tests/InputTest.h"

#include <PantheonApp/Input/EKey.h>
#include <PantheonApp/Input/InputManager.h>

#include <PantheonCore/Debug/Logger.h>
#include <PantheonCore/Utility/ServiceLocator.h>

using namespace PantheonApp::Input;
using namespace PantheonApp::Windowing;

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

    void InputTest::testKeyboard()
    {
        const auto& inputManager = PTH_SERVICE(InputManager);
        const auto& window       = PTH_SERVICE(Window);

        switch (m_frameCount)
        {
        case 0:
        {
            window.m_keyEvent.invoke(EKey::P, 0, EKeyState::PRESSED, EInputModifier::NONE);

            TEST_CHECK(inputManager.isKeyPressed(EKey::P));
            TEST_CHECK(inputManager.isKeyDown(EKey::P));
            TEST_CHECK(!inputManager.isKeyReleased(EKey::P));
            TEST_CHECK(!inputManager.isKeyUp(EKey::P));
            break;
        }
        case 1:
        {
            TEST_CHECK(!inputManager.isKeyPressed(EKey::P));
            TEST_CHECK(inputManager.isKeyDown(EKey::P));
            TEST_CHECK(!inputManager.isKeyReleased(EKey::P));
            TEST_CHECK(!inputManager.isKeyUp(EKey::P));

            window.m_keyEvent.invoke(EKey::P, 0, EKeyState::REPEATED, EInputModifier::NONE);

            TEST_CHECK(!inputManager.isKeyPressed(EKey::P));
            TEST_CHECK(inputManager.isKeyDown(EKey::P));
            TEST_CHECK(!inputManager.isKeyReleased(EKey::P));
            TEST_CHECK(!inputManager.isKeyUp(EKey::P));
            break;
        }
        case 2:
        {
            TEST_CHECK(!inputManager.isKeyPressed(EKey::P));
            TEST_CHECK(inputManager.isKeyDown(EKey::P));
            TEST_CHECK(!inputManager.isKeyReleased(EKey::P));
            TEST_CHECK(!inputManager.isKeyUp(EKey::P));

            window.m_keyEvent.invoke(EKey::P, 0, EKeyState::RELEASED, EInputModifier::NONE);

            TEST_CHECK(!inputManager.isKeyPressed(EKey::P));
            TEST_CHECK(!inputManager.isKeyDown(EKey::P));
            TEST_CHECK(inputManager.isKeyReleased(EKey::P));
            TEST_CHECK(inputManager.isKeyUp(EKey::P));
            break;
        }
        case 3:
        {
            TEST_CHECK(!inputManager.isKeyPressed(EKey::P));
            TEST_CHECK(!inputManager.isKeyDown(EKey::P));
            TEST_CHECK(!inputManager.isKeyReleased(EKey::P));
            TEST_CHECK(inputManager.isKeyUp(EKey::P));
            break;
        }
        default:
            break;
        }
    }

    void InputTest::testMouse()
    {
        const auto& inputManager = PTH_SERVICE(InputManager);
        const auto& window       = PTH_SERVICE(Window);

        switch (m_frameCount)
        {
        case 0:
        {
            window.m_mouseButtonEvent.invoke(EMouseButton::LEFT, EMouseButtonState::PRESSED, EInputModifier::NONE);

            TEST_CHECK(inputManager.isMouseButtonPressed(EMouseButton::LEFT));
            TEST_CHECK(inputManager.isMouseButtonDown(EMouseButton::LEFT));
            TEST_CHECK(!inputManager.isMouseButtonReleased(EMouseButton::LEFT));
            TEST_CHECK(!inputManager.isMouseButtonUp(EMouseButton::LEFT));
            break;
        }
        case 1:
        {
            TEST_CHECK(!inputManager.isMouseButtonPressed(EMouseButton::LEFT));
            TEST_CHECK(inputManager.isMouseButtonDown(EMouseButton::LEFT));
            TEST_CHECK(!inputManager.isMouseButtonReleased(EMouseButton::LEFT));
            TEST_CHECK(!inputManager.isMouseButtonUp(EMouseButton::LEFT));

            window.m_mouseButtonEvent.invoke(EMouseButton::LEFT, EMouseButtonState::RELEASED, EInputModifier::NONE);

            TEST_CHECK(!inputManager.isMouseButtonPressed(EMouseButton::LEFT));
            TEST_CHECK(!inputManager.isMouseButtonDown(EMouseButton::LEFT));
            TEST_CHECK(inputManager.isMouseButtonReleased(EMouseButton::LEFT));
            TEST_CHECK(inputManager.isMouseButtonUp(EMouseButton::LEFT));
            break;
        }
        case 2:
        {
            TEST_CHECK(!inputManager.isMouseButtonPressed(EMouseButton::LEFT));
            TEST_CHECK(!inputManager.isMouseButtonDown(EMouseButton::LEFT));
            TEST_CHECK(!inputManager.isMouseButtonReleased(EMouseButton::LEFT));
            TEST_CHECK(inputManager.isMouseButtonUp(EMouseButton::LEFT));
            break;
        }
        default:
            break;
        }
    }
}
