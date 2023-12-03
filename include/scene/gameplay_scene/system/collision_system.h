#pragma once

#ifndef CRUCIBLE_COLLISION_SYSTEM_H
#define CRUCIBLE_COLLISION_SYSTEM_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
#include <SFML/System/Vector3.hpp>

#include "system.h"
#include "entity_manager.h"
#include "common_constants.h"

class CollisionSystem : public System
{
    public:
        explicit CollisionSystem(EntityManager& entityManager);

        void execute() override;

    private:
        static bool isCollidingAABB(const Component::CShape& entityRect,
                const Component::CShape& otherEntityRect, sf::FloatRect& overlap);

        static void resolveCollision(
                const Component::CShape& entityRectangleShape, Component::CTransform& entityTransform,
                const Component::CShape& otherEntityRectangleShape, const Component::CTransform& otherEntityTransform,
                const sf::FloatRect& overlap);

        static void checkForLightIntersectWithShape(
                Component::CLightSource& lightSource,
                Component::CShape otherEntityRectangleShape,
                size_t shapeLineStartIndex,
                size_t shapeLineEndIndex);

        static void checkForLightIntersectWithWindowBorder(
                Component::CLightSource& lightSource,
                Vec2 windowBorderPosX,
                Vec2 windowBorderPosY);

        static sf::Vector3f getManifold(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal);

        static void applyCollisionManifoldToTransform(
                Component::CTransform& entityTransform,
                const sf::FloatRect& overlap,
                const Vec2& result);

        static float crossProduct(Vec2 a, Vec2 b);

        static Crucible::LightRayIntersect isLineIntersecting(Vec2 vertexA, Vec2 vertexB, Vec2 vertexC, Vec2 vertexD);

    private:
        EntityManager& m_entityManager;
};

#endif //CRUCIBLE_COLLISION_SYSTEM_H