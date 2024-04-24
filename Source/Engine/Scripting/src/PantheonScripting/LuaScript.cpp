#include "PantheonScripting/LuaScript.h"

#include <sol/state.hpp>

namespace PantheonScripting
{
    LuaScript::LuaScript()
        : m_executionOrder(0)
    {
    }

    bool LuaScript::load(const std::string& fileName)
    {
        if (!CHECK(!fileName.empty(), "Attempted to load lua script from empty path"))
            return false;

        std::ifstream file(fileName, std::ios::binary | std::ios::ate);

        if (!CHECK(file.is_open(), "Unable to load lua script - Failed to open file at path \"%s\"", fileName.c_str()))
            return false;

        const std::ifstream::pos_type length = file.tellg();
        file.seekg(0, std::ios::beg);

        m_source.resize(length);
        file.read(m_source.data(), length);
        file.close();

        return !m_source.empty();
    }

    bool LuaScript::save(const std::string& fileName) const
    {
        if (!CHECK(!fileName.empty(), "Attempted to save lua script to empty path"))
            return false;

        std::ofstream file(fileName, std::ios::binary);

        if (!CHECK(file.is_open(), "Unable to save lua script - Failed to open file at path \"%s\"", fileName.c_str()))
            return false;

        file << m_source;

        return CHECK(!file.bad(), "Failed to save lua script to \"%s\"", fileName.c_str());
    }

    bool LuaScript::toBinary(std::vector<char>& output) const
    {
        return CHECK(serializeString(m_source, output), "Failed to serialize lua script source")
            && CHECK(writeNumber(m_executionOrder, output), "Failed to serialize lua script execution order");
    }

    size_t LuaScript::fromBinary(const char* data, const size_t length)
    {
        const size_t offset = deserializeString(m_source, data, length);

        if (!CHECK(offset != 0, "Failed to deserialize lua script source"))
            return 0;

        if (!CHECK(length >= offset, "Failed to deserialize lua script execution order - Invalid offset"))
            return 0;

        const size_t readBytes = readNumber(m_executionOrder, data + offset, length - offset);

        return CHECK(readBytes != 0, "Failed to deserialize lua script execution order") ? offset + readBytes : 0;
    }

    std::string_view LuaScript::getSource() const
    {
        return m_source;
    }

    LuaScript::OrderT LuaScript::getExecutionOrder() const
    {
        return m_executionOrder;
    }
}
