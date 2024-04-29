#include "PantheonScripting/LuaScript.h"

#include "PantheonCore/Utility/FileSystem.h"
#include "PantheonCore/Serialization/Serializer.h"

#include <sol/state.hpp>

using namespace PantheonCore::Utility;
using namespace PantheonCore::Serialization;

namespace PantheonScripting
{
    LuaScript::LuaScript()
        : m_executionOrder(0)
    {
    }

    bool LuaScript::load(const std::string& path)
    {
        if (!CHECK(!path.empty(), "Attempted to load lua script from empty path"))
            return false;

        std::ifstream file(path, std::ios::binary | std::ios::ate);

        if (!CHECK(file.is_open(), "Unable to load lua script - Failed to open file at path \"%s\"", path.c_str()))
            return false;

        const std::ifstream::pos_type length = file.tellg();
        file.seekg(0, std::ios::beg);

        m_source.resize(length);
        file.read(m_source.data(), length);
        file.close();

        return !m_source.empty() && loadMeta(getMetaPath(path));
    }

    bool LuaScript::save(const std::string& path) const
    {
        return saveMeta(getMetaPath(path));
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

    void LuaScript::SetExecutionOrder(const OrderT p_executionOrder)
    {
        m_executionOrder = p_executionOrder;
    }

    bool LuaScript::loadMeta(const std::string& p_path)
    {
        if (!pathExists(p_path))
            return true;

        JsonDocument json = loadJsonFile(p_path);

        const auto it = json.FindMember("order");

        if (!CHECK(it != json.MemberEnd(), "Unable to deserialize lua script meta data - Missing execution order"))
            return false;

        if (!CHECK(it->value.IsInt(), "Unable to deserialize lua script execution order - Json value should be an int"))
            return false;

        m_executionOrder = static_cast<OrderT>(it->value.GetInt());

        return true;
    }

    bool LuaScript::saveMeta(const std::string& p_path) const
    {
        std::ofstream fs(p_path);

        if (!CHECK(fs.is_open(), "Unable to open lua script meta file at path \"%s\"", p_path.c_str()))
            return false;

        JsonOStream    jos(fs);
        JsonFileWriter writer(jos);

        writer.StartObject();

        writer.Key("order");
        writer.Int(m_executionOrder);

        if (!writer.EndObject() || !ASSUME(writer.IsComplete(), "Failed to save lua script meta - Generated json is incomplete"))
            return false;

        return CHECK(!fs.bad(), "Failed to write lua script meta data to \"%s\"", p_path.c_str());
    }
}
