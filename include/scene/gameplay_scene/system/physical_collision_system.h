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

class PhysicalCollisionSystem : public System
{
    public:
        explicit PhysicalCollisionSystem(EntityManager& entityManager);

        void execute() override;

    private:
        static bool isCollidingAABB(const Component::CTile& entityTile,
                const std::shared_ptr<sf::VertexArray>& otherRectVertices, sf::FloatRect& overlap);

        static void resolveCollision(Component::CTile& entityTile, Component::CTransform& entityTransform,
                const Crucible::Vec2& otherEntityPositionVec, const sf::FloatRect& overlap);

        static void resolvePhysicalCollisionsForObjectLayer(Component::CCollider& entityCollider, Component::CTile& entityTile,
                Component::CTransform& entityTransform, ObjectLayer& lightingObjectLayer) ;

        static void resolvePhysicalCollisions(Component::CTile& entityRectangleShape,
                Component::CTransform& entityTransform, Component::CCollider entityCollider,
                const Crucible::Vec2& otherRectPos, const std::shared_ptr<sf::VertexArray>& otherRectVertices);

        void checkForLevelObjectLayerCollisions(Component::CCollider& entityCollider, Component::CTile& entityTile,
                Component::CTransform& entityTransform) const;

        void checkForOtherCollidableEntities(std::vector<Entity>& entities, const Entity& entity,
                Component::CCollider& entityCollider, Component::CTile& entityRectangleShape,
                Component::CTransform& entityTransform) const;

        static void updateShapeVertexPositions(const Component::CTransform& entityTransform,
                Component::CTile& entityTile);

        static void applyCollisionOverlapToEntityTransform(Component::CTransform& entityTransform,
                sf::Vector3f manifoldDist, sf::Vector2f collisionOverlap);

        static sf::Vector3f getManifold(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal);

        static void applyCollisionManifoldToTransform(Component::CTransform& cTransform,
                const sf::FloatRect& overlap, const Crucible::Vec2& result);

    private:
        EntityManager& m_entityManager;
};

#endif //CRUCIBLE_PHYSICAL_COLLISION_SYSTEM_H