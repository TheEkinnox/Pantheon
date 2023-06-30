#pragma once

#include "PantheonCore/Utility/utility.h"
#include <stdexcept>

namespace PantheonEngine::Core::Utility
{
	template<typename ... Args>
	std::string formatString(const char* const format, Args ... args)
	{
		// If no template parameters are passed
		// return the format string as is to avoid unnecessary allocation
		if constexpr (sizeof...(Args) == 0)
		{
			return format;
		}
		else
		{
			// get the formatted text's size
			const int bufferSize = std::snprintf(nullptr, 0, format, args...) + 1;

			if (bufferSize <= 0)
				throw std::runtime_error("Unable to print to log - formatting failed.");

			// Create a buffer of the computed size
			std::vector<char> buffer;
			buffer.reserve(bufferSize);

			// Write the formatted string in the buffer
			std::snprintf(buffer.data(), bufferSize, format, args...);

			// Copy the buffer data into an std::string
			std::string message(buffer.data(), buffer.data() + bufferSize - 1);

			return message;
		}
	}
}
