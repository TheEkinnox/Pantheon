#pragma once
#include <string>

namespace PantheonTest
{
    class ITest
    {
    public:
        virtual ~ITest() = default;

        void start();

        void preUpdate();

        void update();

        void postUpdate();

        void fixedUpdate();

        void stop();

        bool isDone() const;

    protected:
        ITest();

        explicit ITest(std::string name);

        virtual void onStart();

        virtual void onPreUpdate();

        virtual void onUpdate();

        virtual void onPostUpdate();

        virtual void onFixedUpdate();

        virtual void onStop();

        void complete();

    private:
        std::string m_name;
        bool        m_isDone;
    };
}
