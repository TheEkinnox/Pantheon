#pragma once

namespace PantheonScripting
{
    struct ScriptingFunctions
    {
        static constexpr const char* INIT         = "onInit";
        static constexpr const char* START        = "onStart";
        static constexpr const char* UPDATE       = "onUpdate";
        static constexpr const char* FIXED_UPDATE = "onFixedUpdate";
        static constexpr const char* STOP         = "onStop";
        static constexpr const char* DESTROY      = "onDestroy";
    };
}
