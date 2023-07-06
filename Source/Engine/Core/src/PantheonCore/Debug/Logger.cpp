#include "PantheonCore/Debug/Logger.h"

#include "PantheonCore/Utility/ServiceLocator.h"
#include "PantheonCore/Utility/macros.h"

namespace PantheonEngine::Core::Debug
{
#pragma region BINDINGS

    extern "C"
    {
        EXPORT_SERVICE_FUNC(/**/, /**/, Logger, void, print, ARGS(const char* message, bool isError),
            ARGS(message, isError))
        EXPORT_SERVICE_FUNC(/**/, /**/, Logger, void, openFile, const char* filePath, filePath)
        EXPORT_SERVICE_FUNC_NO_PARAMS(/**/, /**/, Logger, void, closeFile)
    }

#pragma endregion
}
