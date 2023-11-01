#pragma once
#include <istream>
#include <vector>

namespace PantheonEngine::Core::Assets
{
    class Asset
    {
        static constexpr int  SIZE_BITS = 64;
        static constexpr char DATA_SEPARATOR = '\f';
        static constexpr char ENTRY_SEPARATOR = '\0';

    public:
        /**
         * \brief Creates a default asset
         */
        Asset();

        /**
         * \brief Creates an asset with the given values
         * \param type The asset's type
         * \param guid The asset's guid
         * \param path The asset's path
         */
        Asset(std::string type, std::string guid, std::string path);

        /**
         * \brief Creates a copy of the given asset
         * \param other The asset to copy
         */
        Asset(const Asset& other) = default;

        /**
         * \brief Creates a move copy of the given asset
         * \param other The asset to move
         */
        Asset(Asset&& other) noexcept;

        /**
         * \brief Destroys the asset
         */
        virtual ~Asset() = default;

        /**
         * \brief Creates a copy of the given asset
         * \param other The asset to copy
         */
        Asset& operator=(const Asset& other);

        /**
         * \brief Creates a move copy of the given asset
         * \param other The asset to move
         */
        Asset& operator=(Asset&& other) noexcept;

        /**
         * \brief Provides read access to the asset's size
         * \return The asset's size
         */
        uint64_t getSize() const;

        /**
         * \brief Provides read access to the asset's guid
         * \return The asset's guid
         */
        const char* getGuid() const;

        /**
         * \brief Provides read access to the asset's type
         * \return The asset's type
         */
        const char* getType() const;

        /**
         * \brief Provides read access to the asset's path
         * \return The asset's path
         */
        const char* getPath() const;

        /**
         * \brief Checks whether the asset is valid or not
         * \return True if the asset is valid. False otherwise
         */
        virtual bool isValid() const;

        /**
         * \brief Writes the asset's data in a memory buffer and updates the asset's size
         * \param output The target memory buffer
         * \return True if the data could be read. False otherwise
         */
        virtual bool getData(std::vector<char>& output);

        /**
         * \brief Writes the asset's data in a memory buffer
         * \param output The target memory buffer
         * \return True if the data could be read. False otherwise
         */
        virtual bool getData(std::vector<char>& output) const;

        /**
         * \brief Reads the asset's data from the given input stream
         * \param is The stream to read the asset data from
         * \param asset The asset in which the data should be read
         * \return A reference to the input stream
         */
        friend std::istream& operator>>(std::istream& is, Asset& asset);

        /**
         * \brief Writes the given asset in the given output stream
         * \param os The stream in which the asset should be written
         * \param asset The asset to write
         * \return A reference to the output stream
         */
        friend std::ostream& operator<<(std::ostream& os, const Asset& asset);

    protected:
        std::string m_guid;
        std::string m_type;
        std::string m_path;
        uint64_t    m_size;
    };
}
