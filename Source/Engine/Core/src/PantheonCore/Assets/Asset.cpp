#include "PantheonCore/Assets/Asset.h"

#include <fstream>

#include "PantheonCore/Debug/Logger.h"
#include "PantheonCore/Utility/ByteOrder.inl"

namespace PantheonEngine::Core::Assets
{
    Asset::Asset() : m_size(0)
    {
    }

    Asset::Asset(std::string type, std::string guid, std::string path)
        : m_guid(std::move(guid)), m_type(std::move(type)), m_path(std::move(path)), m_size(0)
    {
    }

    Asset::Asset(Asset&& other) noexcept
        : m_guid(std::move(other.m_guid)), m_type(std::move(other.m_type)), m_path(std::move(other.m_path)), m_size(other.m_size)
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

    bool Asset::isValid() const
    {
        const std::ifstream fs(getPath());
        return fs.is_open();
    }

    bool Asset::getData(std::vector<char>& output)
    {
        m_size = 0;

        const size_t startSize = output.size();
        const Asset& tmp = *this;

        if (!tmp.getData(output))
            return false;

        m_size = output.size() - startSize;
        return true;
    }

    bool Asset::getData(std::vector<char>& output) const
    {
        std::ifstream file(m_path, std::ifstream::in | std::ifstream::binary);

        if (!file.is_open())
        {
            DEBUG_LOG_ERROR("Unable to get asset data - Failed to open file at path \"%s\"", m_path.c_str());
            return false;
        }

        // get file size
        file.seekg(0, std::ifstream::end);
        const uint64_t fileSize = file.tellg();
        file.seekg(0, std::ifstream::beg);

        if (fileSize > 0)
        {
            // read file
            output.resize(output.size() + fileSize);
            file.read(output.data(), static_cast<std::streamsize>(fileSize));
        }

        file.close();
        return true;
    }

    std::istream& operator>>(std::istream& is, Asset& asset)
    {
        is.read(reinterpret_cast<char*>(&asset.m_size), sizeof asset.m_size);

        asset.m_size = Utility::fromBigEndian(asset.m_size);

        std::string str;
        char        c;

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
        const auto leSize = Utility::toBigEndian(asset.m_size);

        os.write(reinterpret_cast<const char*>(&leSize), sizeof asset.m_size);
        os << asset.m_guid << Asset::DATA_SEPARATOR << asset.m_type << Asset::DATA_SEPARATOR << asset.m_path <<
            Asset::ENTRY_SEPARATOR << std::flush;

        return os;
    }
}
