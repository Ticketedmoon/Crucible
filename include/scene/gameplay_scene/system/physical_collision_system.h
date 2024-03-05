#pragma once

#ifndef CRUCIBLE_PHYSICAL_COLLISION_SYSTEM_H
#define CRUCIBLE_PHYSICAL_COLLISION_SYSTEM_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
#include <SFML/System/Vector3.hpp>

#include "system.h"
#include "entity_manager.h"
#include "core/engine/common_constants.h"
#include "core/manager/level_manager.h"
#include "game_engine.h"

class PhysicalCollisionSystem : public System
{
    public:
        explicit PhysicalCollisionSystem(GameEngine& gameEngine, EntityManager& entityManager);

        void execute() override;

    private:
        static bool isCollidingAABB(
                const Component::CTile& entityTile,
                const sf::FloatRect& otherObjectBounds,
                sf::FloatRect& overlap);

        void resolveCollisionByManifold(Component::CTile& entityTile, Component::CTransform& entityTransform,
                const Crucible::Vec2& otherEntityPositionVec, const sf::FloatRect& overlap);

        void resolvePhysicalCollisionsForObjectLayer(const Entity& entity, ObjectLayer& objectLayer);

        void resolvePhysicalCollisions(Entity& entity, Entity& otherEntity);

        void checkForLevelObjectLayerCollisions(const Entity& entity,
                Component::CCollider& entityCollider);

        void checkForOtherCollidableEntities(std::vector<Entity>& entities, Entity& entity);

        void updateShapeVertexPositions(const Component::CTransform& entityTransform,
                Component::CTile& entityTile);

        void applyCollisionOverlapToEntityTransform(Component::CTransform& entityTransform,
                sf::Vector3f manifoldDist, sf::Vector2f collisionOverlap);

        sf::Vector3f getManifold(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal);

        void applyCollisionManifoldToTransform(Component::CTransform& cTransform,
                const sf::FloatRect& overlap, const Crucible::Vec2& result);

    private:
        GameEngine& m_gameEngine;
        EntityManager& m_entityManager;
};

#endif //CRUCIBLE_PHYSICAL_COLLISION_SYSTEM_H