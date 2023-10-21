#include "PantheonCore/Entities/Scene.h"

namespace PantheonEngine::Core::Entities
{
    void Scene::update()
    {
        for (const auto& node : getNodes())
            node->update();
    }
}
