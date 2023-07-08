#include "PantheonCore/Assets/Asset.h"

#include <filesystem>

namespace PantheonEngine::Core::Assets
{
    Asset::Asset() : m_size(0)
    {
    }

    Asset::Asset(const char* guid, const char* type, const char* path) :
        m_guid(guid), m_type(type), m_path(path)
    {
        // No try catch - this should throw if an invalid file is provided
        m_size = std::filesystem::file_size(path);
    }

    Asset::Asset(Asset&& other) noexcept
        : m_size{ other.m_size },
          m_guid{ std::move(other.m_guid) },
          m_type{ std::move(other.m_type) },
          m_path{ std::move(other.m_path) }
    {
    }

    Asset& Asset::operator=(const Asset& other)
    {
        if (this == &other)
            return *this;

        m_size = other.m_size;
        m_guid = other.m_guid;
        m_type = other.m_type;
        m_path = other.m_path;

        return *this;
    }

    Asset& Asset::operator=(Asset&& other) noexcept
    {
        if (this == &other)
            return *this;

        m_size = other.m_size;
        m_guid = std::move(other.m_guid);
        m_type = std::move(other.m_type);
        m_path = std::move(other.m_path);

        return *this;
    }

    uint64_t Asset::getSize() const
    {
        return m_size;
    }

    const char* Asset::getGuid() const
    {
        return m_guid.c_str();
    }

    const char* Asset::getType() const
    {
        return m_type.c_str();
    }

    const char* Asset::getPath() const
    {
        return m_path.c_str();
    }

    std::istream& operator>>(std::istream& is, Asset& asset)
    {
        is.read(reinterpret_cast<char*>(&asset.m_size), sizeof asset.m_size);

        std::string str;
        char c;

        while (is.get(c) && c != Asset::DATA_SEPARATOR && c != Asset::ENTRY_SEPARATOR)
            str += c;

        asset.m_guid = str;
        str.clear();

        while (is.get(c) && c != Asset::DATA_SEPARATOR && c != Asset::ENTRY_SEPARATOR)
            str += c;

        asset.m_type = str;
        str.clear();

        while (is.get(c) && c != Asset::DATA_SEPARATOR && c != Asset::ENTRY_SEPARATOR)
            str += c;

        asset.m_path = str;

        // make sure eof can be reached
        is.get();

        if (!is.eof())
            is.unget();

        return is;
    }

    std::ostream& operator<<(std::ostream& os, const Asset& asset)
    {
        os.write(reinterpret_cast<const char*>(&asset.m_size), sizeof asset.m_size);
        os << asset.m_guid << Asset::DATA_SEPARATOR << asset.m_type << Asset::DATA_SEPARATOR << asset.m_path <<
            Asset::ENTRY_SEPARATOR << std::flush;

        return os;
    }
}
