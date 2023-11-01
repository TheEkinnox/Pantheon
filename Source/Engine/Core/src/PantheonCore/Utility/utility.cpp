#include "PantheonCore/Utility/utility.h"

#include <sstream>

namespace PantheonEngine::Core::Utility
{
    std::vector<std::string> splitString(const std::string& str, const char* delimiter, const bool includeEmpty)
    {
        std::vector<std::string> result{};

        if (str.empty())
        {
            if (includeEmpty)
                result.push_back(str);

            return result;
        }

        size_t start = 0;
        size_t end = str.find(delimiter, start);

        while (start < str.size())
        {
            std::string token = str.substr(start, end - start);

            if (includeEmpty || !token.empty())
                result.push_back(std::move(token));

            if (end == std::string::npos)
                break;

            start = end + strlen(delimiter);
            end = str.find(delimiter, start);
        }

        return result;
    }

    std::string sizeToStr(double size, const bool addInitialValue)
    {
        const uint64_t initialValue = static_cast<uint64_t>(size);

        constexpr int         unitsCount = 5;
        constexpr const char* units[unitsCount] = { "B", "KB", "MB", "GB", "TB" };

        int unitIndex = 0;

        while (size >= 1024. && unitIndex < unitsCount - 1)
        {
            size /= 1024.;
            unitIndex++;
        }

        std::ostringstream oss;
        oss << size << units[unitIndex];

        if (addInitialValue)
            oss << " (" << initialValue << units[0] << ')';

        return oss.str();
    }
}
