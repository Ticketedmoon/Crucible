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
                TextureManager& textureManager,
                sf::Clock& gameClock);

        void createPlayer();
        void createGuard(
                const std::string& lightingObjectLayerName,
                const std::string& pathingObjectLayerName);
        void createProjectile(Crucible::Vec2 startPosition, Crucible::Vec2 targetPosition);

    private:
        static Component::CLightSource createLightSource(
                Component::CTransform& playerTransform,
                const std::string& layerName);

    private:
        static inline const std::string PLAYER_SPRITE_SHEET_PATH = "resources/assets/texture/player/player_crucible_16x32_6pr_idle.png";
        static inline const std::string SHADOW_BALL_SPRITE_SHEET_PATH = "resources/assets/texture/guard/shadow_ball_32x28.png";

        EntityManager& m_entityManager;
        TextureManager& m_textureManager;
        sf::Clock& m_gameClock;
};


#endif //CRUCIBLE_ENTITY_SPAWNER_H
