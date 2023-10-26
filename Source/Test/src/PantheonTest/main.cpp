#if defined(_DEBUG) && defined(_MSC_VER)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "PantheonTest/TestApplication.h"

using namespace PantheonTest;

int main(int argc, char* argv[])
{
#if defined(_DEBUG) && defined(_MSC_VER)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    TestApplication app;
    app.run(argc, argv);

    return 0;
}
