#include "PantheonCore/Debug/Logger.h"
#include "PantheonCore/Utility/macros.h"

namespace PantheonEngine::Core::Debug
{
#pragma region BINDINGS

    extern "C"
    {
        PANTHEON_API void Logger_print(const char* message, const bool isError)
        {
            return Logger::getInstance().print(message, isError);
        }

        PANTHEON_API void Logger_setFile(const char* filePath)
        {
            return Logger::getInstance().setFile(filePath);
        }
    }

#pragma endregion
}
