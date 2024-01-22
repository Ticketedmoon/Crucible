#pragma once

#ifndef CRUCIBLE_ENTITY_SPAWNER_H
#define CRUCIBLE_ENTITY_SPAWNER_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>

#include "system.h"
#include "entity_manager.h"
#include "common_constants.h"
#include "vertex.h"
#include "ray.h"

class EntitySpawner
{
    public:
        explicit EntitySpawner(EntityManager& entityManager);

        void spawnPlayer();
        void spawnWall(Crucible::Vec2 position, Crucible::Vec2 dimensions, bool isCollidable, sf::Color wallColor);

    private:
        std::vector<Crucible::Ray> createRays(Component::CTransform& playerTransform);

    private:
        EntityManager& m_entityManager;
};


#endif //CRUCIBLE_ENTITY_SPAWNER_H
