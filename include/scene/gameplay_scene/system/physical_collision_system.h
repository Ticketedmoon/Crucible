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
        static bool isCollidingAABB(const Component::CShape& entityRect,
                const Component::CShape& otherEntityRect, sf::FloatRect& overlap);

        static void resolveCollision(Component::CShape& entityRectangleShape,
                Component::CTransform& entityTransform,
                Component::CShape& otherEntityRectangleShape, Component::CTransform& otherEntityTransform,
                const sf::FloatRect& overlap);

        static void resolvePhysicalCollisions(Component::CShape& entityRectangleShape,
                Component::CTransform& entityTransform, Component::CTransform& otherEntityTransform,
                Component::CShape& otherEntityRectangleShape);

        static void updateShapeVertexPositions(const Component::CTransform& entityTransform,
                Component::CShape& entityRectangleShape);

        static void applyCollisionOverlapToEntityTransform(Component::CTransform& entityTransform,
                sf::Vector3f manifoldDist, sf::Vector2f collisionOverlap);

        static sf::Vector3f getManifold(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal);

        static void applyCollisionManifoldToTransform(Component::CTransform& cTransform, Component::CTransform& otherCTransform,
                const sf::FloatRect& overlap, const Crucible::Vec2& result);

    private:
        EntityManager& m_entityManager;
};

#endif //CRUCIBLE_PHYSICAL_COLLISION_SYSTEM_H