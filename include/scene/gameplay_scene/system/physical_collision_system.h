#pragma once

#ifndef CRUCIBLE_PHYSICAL_COLLISION_SYSTEM_H
#define CRUCIBLE_PHYSICAL_COLLISION_SYSTEM_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
#include <SFML/System/Vector3.hpp>

#include "system.h"
#include "entity_manager.h"
#include "common_constants.h"
#include "level_manager.h"
#include "game_engine.h"

class PhysicalCollisionSystem : public System
{
    public:
        explicit PhysicalCollisionSystem(GameEngine& gameEngine, EntityManager& entityManager);

        void execute() override;

    private:
        bool isCollidingAABB(const Component::CTile& entityTile,
                const std::shared_ptr<sf::VertexArray>& otherRectVertices, sf::FloatRect& overlap);

        void resolveCollision(Component::CTile& entityTile, Component::CTransform& entityTransform,
                const Crucible::Vec2& otherEntityPositionVec, const sf::FloatRect& overlap);

        void resolvePhysicalCollisionsForObjectLayer(
                Component::CCollider& entityCollider,
                const Entity& entity,
                ObjectLayer& lightingObjectLayer);

        void resolvePhysicalCollisions(Component::CTile& entityTile,
                Component::CTransform& entityTransform,
                Component::CCollider entityCollider,
                const Crucible::Vec2& otherRectPos,
                const std::shared_ptr<sf::VertexArray>& otherTileVertices);

        void checkForLevelObjectLayerCollisions(const Entity& entity,
                Component::CCollider& entityCollider);

        void checkForOtherCollidableEntities(std::vector<Entity>& entities,
                const Entity& entity,
                Component::CCollider& entityCollider);

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