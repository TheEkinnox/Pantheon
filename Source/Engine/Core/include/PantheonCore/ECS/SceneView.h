#pragma once
#include "PantheonCore/ECS/SceneViewIterator.h"
#include "PantheonCore/Utility/TypeTraits.h"

namespace PantheonCore::ECS
{
    class Scene;

    template <class... Components>
    class SceneView
    {
        static_assert(sizeof...(Components) > 0);
        static_assert(!Utility::HasDuplicates<Components...>);

        template <typename T>
        static constexpr bool Has = Utility::IsOneOf<T, Components...> || Utility::IsOneOf<std::remove_const_t<T>, Components...>;

        static constexpr bool IsAllConst = Utility::IsAllConst<Components...>;

        template <typename T>
        static constexpr size_t IndexOf = Utility::IndexOf<T, Components...>;

    public:
        using iterator = SceneViewIterator<IsAllConst, Components...>;
        using const_iterator = SceneViewIterator<true, Components...>;
        using SceneRef = std::conditional_t<IsAllConst, const Scene&, Scene&>;
        using ScenePtr = std::conditional_t<IsAllConst, const Scene*, Scene*>;
        using EntityStorageRef = std::conditional_t<IsAllConst, const EntityStorage&, EntityStorage&>;

        /**
         * \brief Creates a view for the given scene
         * \param scene The target scene
         */
        explicit SceneView(SceneRef scene);

        /**
         * \brief Creates a copy of the given scene view
         * \param other The scene view to copy
         */
        SceneView(const SceneView& other) = default;

        /**
         * \brief Creates a move copy of the given scene view
         * \param other The scene view to move
         */
        SceneView(SceneView&& other) noexcept = default;

        /**
         * \brief Destroys the scene view
         */
        ~SceneView() = default;

        /**
         * \brief Assigns a copy of the given scene view to this one
         * \param other The scene view to copy
         * \return A reference to the modified scene view
         */
        SceneView& operator=(const SceneView& other) = default;

        /**
         * \brief Moves the given scene view into this one
         * \param other The scene view to move
         * \return A reference to the modified scene view
         */
        SceneView& operator=(SceneView&& other) noexcept = default;

        /**
         * \brief Reloads the view's linked storages
         */
        void refresh();

        /**
         * \brief Gets the components of the given types owned by the given entity
         * \tparam T The component types to fetch
         * \param owner The searched components' owner
         * \return A tuple of pointers to the found components on success. Not found components are null
         */
        template <typename... T>
        std::enable_if_t<(sizeof...(T) > 1), std::tuple<T*...>> get(Entity owner);

        /**
         * \brief Gets the component of the given type owned by the given entity
         * \tparam T The component type to fetch
         * \param owner The searched component's owner
         * \return A pointer to the found component on success. Nullptr otherwise
         */
        template <typename T>
        T* get(Entity owner);

        /**
         * \brief Gets the components handled by the scene view owned by the given entity
         * \param owner The searched components' owner
         * \return A tuple of pointers to the found components on success. Not found components are null
         */
        std::tuple<Components*...> get(Entity owner);

        /**
         * \brief Gets the storage for the given component type
         * \tparam T The target component type
         * \return A reference to the storage for the given component type
         */
        template <typename T>
        ComponentStorage<T>& getStorage();

        /**
         * \brief Sets the storage for the given component type
         * \tparam T The target component type
         * \param storage The new storage for the given component type
         */
        template <typename T>
        void setStorage(ComponentStorage<T>& storage);

        /**
         * \brief Gets an iterator to the start of the scene view
         * \return An iterator to the start of the scene view
         */
        iterator begin();

        /**
         * \brief Gets an iterator to the end of the scene view
         * \return An iterator to the end of the scene view
         */
        iterator end();

        /**
         * \brief Gets a constant iterator to the start of the scene view
         * \return A constant iterator to the start of the scene view
         */
        const_iterator begin() const;

        /**
         * \brief Gets a constant iterator to the end of the scene view
         * \return A constant iterator to the end of the scene view
         */
        const_iterator end() const;

    private:
        ScenePtr                                     m_scene;
        std::tuple<ComponentStorage<Components>*...> m_storages;

        /**
         * \brief Sets up the view's component storages for the current scene
         * \tparam T The current component type
         * \tparam Remainder The remaining component types
         */
        template <typename T, typename... Remainder>
        void initializeView();
    };
}

#include "PantheonCore/ECS/SceneView.inl"
