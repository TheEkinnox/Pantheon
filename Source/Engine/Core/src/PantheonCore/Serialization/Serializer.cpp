#include "PantheonCore/Serialization/Serializer.h"

#include <rapidjson/istreamwrapper.h>

#if PTH_USE_ASSERTION
#include <rapidjson/error/en.h>
#endif

namespace PantheonCore::Serialization
{
    JsonDocument loadJsonFile(const std::string& path)
    {
        std::ifstream fs(path);

        if (!CHECK(fs.is_open(), "Unable to open json file at path \"%s\"", path.c_str()))
            return {};

        rapidjson::IStreamWrapper isw(fs);

        rapidjson::Document json;
        json.ParseStream(isw);

        if (!CHECK(!json.HasParseError(), "Unable to parse json from file - Parse error %d (%s)",
                json.GetParseError(), rapidjson::GetParseError_En(json.GetParseError())))
            return {};

        return json;
    }
}
