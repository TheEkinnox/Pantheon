#pragma once
#include <PantheonCore/Resources/IResource.h>

namespace PantheonScripting
{
    class LuaScript final : public PantheonCore::Resources::IResource
    {
        REGISTERED_RESOURCE_BODY()

    public:
        using OrderT = int16_t;

        /**
         * \brief Creates an empty lua script
         */
        LuaScript();

        /**
         * \brief Creates a copy of the given lua script
         * \param other The lua script to copy
         */
        LuaScript(const LuaScript& other) = default;

        /**
         * \brief Creates a move copy of the given lua script
         * \param other The lua script to move
         */
        LuaScript(LuaScript&& other) noexcept = default;

        /**
         * \brief Destroys the lua script
         */
        ~LuaScript() override = default;

        /**
         * \brief Assigns a copy of the given lua script to this one
         * \param other The lua script to copy
         * \return A reference to the modified lua script
         */
        LuaScript& operator=(const LuaScript& other) = default;

        /**
         * \brief Moves the given lua script into this one
         * \param other The lua script to move
         * \return A reference to the modified lua script
         */
        LuaScript& operator=(LuaScript&& other) noexcept = default;

        /**
         * \brief Loads the lua script from the given path
         * \param path The script's path
         * \return True on success. False otherwise
         */
        bool load(const std::string& path) override;

        /**
         * \brief Initializes the lua script
         * \return True on success. False otherwise
         */
        bool init() override
        {
            return true;
        }

        /**
         * \brief Saves the meta data of the script at the given path
         * \param path The script's path
         * \return True on success. False otherwise
         */
        bool save(const std::string& path) const override;

        /**
         * \brief Serializes the lua script to a byte array
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        bool toBinary(std::vector<char>& output) const override;

        /**
         * \brief Deserializes the lua script from the given memory buffer
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return The number of deserialized bytes on success. 0 otherwise.
         */
        size_t fromBinary(const char* data, size_t length) override;

        /**
         * \brief Gets the lua script's source code
         * \return The script's source
         */
        std::string_view getSource() const;

        /**
         * \brief Gets the lua script's execution order
         * \return The script's execution order
         */
        OrderT getExecutionOrder() const;

        /**
         * \brief Sets the lua script's execution order
         * \param p_executionOrder The script's new execution order
         */
        void SetExecutionOrder(OrderT p_executionOrder);

    private:
        std::string m_source;
        OrderT      m_executionOrder;

        /**
         * \brief Loads the lua script's meta data from the given path
         * \param p_path The lua script's meta path
         * \return True on success. False otherwise
         */
        bool loadMeta(const std::string& p_path);

        /**
         * \brief Saves the lua script's meta data to the given path
         * \param p_path The lua script's meta path
         * \return True on success. False otherwise
         */
        bool saveMeta(const std::string& p_path) const;
    };
}
