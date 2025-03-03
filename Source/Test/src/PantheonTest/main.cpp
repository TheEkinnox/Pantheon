#include "PantheonCore/Utility/CoreDefines.h"

#if USING(PTH_TARGET_DEBUG) && USING(PTH_COMPILER_MSVC)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "PantheonTest/TestApplication.h"

using namespace PantheonTest;

int main(const int argc, char* argv[])
{
#if USING(PTH_TARGET_DEBUG) && USING(PTH_COMPILER_MSVC)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    TestApplication app;
    app.run(argc, argv);

    return 0;
}
