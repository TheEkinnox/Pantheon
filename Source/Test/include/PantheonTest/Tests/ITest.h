#pragma once

namespace PantheonTest
{
    class ITest
    {
    public:
        virtual ~ITest() = default;

        virtual void run() = 0;
    };
}
