#if defined(_DEBUG) && defined(_MSC_VER)
    #include <crtdbg.h>
#endif

#include "PantheonTest/TestApplication.h"

using namespace PantheonEngine::Test;

int main(int argc, char* argv[])
{
#if defined(_DEBUG) && defined(_MSC_VER)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    TestApplication app;
    app.run(argc, argv);

    return 0;
}
