#pragma once
#include "PantheonTest/Tests/ITest.h"

namespace PantheonTest
{
    class InputTest final : public ITest
    {
    public:
        InputTest();
        explicit InputTest(const std::string& name);

    protected:
        void onUpdate() override;
        void onPostUpdate() override;

    private:
        int m_frameCount = 0;

        void testKeyboard() const;
        void testMouse() const;
    };
}
