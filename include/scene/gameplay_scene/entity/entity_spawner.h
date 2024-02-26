#pragma once

#ifndef CRUCIBLE_ENTITY_SPAWNER_H
#define CRUCIBLE_ENTITY_SPAWNER_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>

#include "core/manager/level_manager.h"
#include "system.h"
#include "entity_manager.h"
#include "core/engine/common_constants.h"
#include "vertex.h"
#include "ray.h"
#include "tile.h"

class EntitySpawner
{
    public:
        explicit EntitySpawner(
                EntityManager& entityManager,
                TextureManager& textureManager);

        void createPlayer();
        void createGuard(
                const std::string& lightingObjectLayerName,
                const std::string& pathingObjectLayerName);

    private:
        static Component::CLightSource createLightSource(
                Component::CTransform& playerTransform,
                const std::string& layerName);

    private:
        EntityManager& m_entityManager;
        TextureManager& m_textureManager;
};


#endif //CRUCIBLE_ENTITY_SPAWNER_H
