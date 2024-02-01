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

class PhysicalCollisionSystem : public System
{
    public:
        explicit PhysicalCollisionSystem(EntityManager& entityManager);

        void execute() override;

    private:
        static bool isCollidingAABB(const Component::CTile& entityTile,
                const Component::CTile& otherTile, sf::FloatRect& overlap);

        static void resolveCollision(Component::CTile& entityTile,
                Component::CTransform& entityTransform,
                Component::CTile& otherEntityTile, Component::CTransform& otherEntityTransform,
                const sf::FloatRect& overlap);

        static void resolvePhysicalCollisions(Component::CTile& entityRectangleShape,
                Component::CTransform& entityTransform, Component::CCollider entityCollider,
                Component::CTransform& otherEntityTransform, Component::CTile& otherEntityRectangleShape);

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