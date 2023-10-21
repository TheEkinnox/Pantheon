#pragma once
#include "PantheonCore/Entities/Entity.h"
#include "PantheonCore/DataStructure/Graph.h"

namespace PantheonEngine::Core::Entities
{
    class Scene : public DataStructure::Graph<Entity>
    {
    public:
        virtual void update();
    };
}
