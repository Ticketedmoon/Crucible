#pragma once

#ifndef CRUCIBLE_ENTITY_SPAWNER_H
#define CRUCIBLE_ENTITY_SPAWNER_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>

#include "system.h"
#include "entity_manager.h"
#include "common_constants.h"
#include "vertex.h"

class EntitySpawner
{
    public:
        explicit EntitySpawner(EntityManager& entityManager);

        void spawnPlayer();
        void spawnWall(Vec2 position, Vec2 dimensions);

    private:
        EntityManager& m_entityManager;
};


#endif //CRUCIBLE_ENTITY_SPAWNER_H
