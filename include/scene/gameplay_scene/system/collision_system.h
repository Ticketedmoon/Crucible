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
                Component::CCollidable& otherEntityCollider,
                const sf::FloatRect& overlap);

        static void resolvePhysicalCollisions(const Component::CShape& entityRectangleShape,
                Component::CTransform& entityTransform, Component::CCollidable& entityCollider,
                const Component::CTransform& otherEntityTransform,
                const Component::CShape& otherEntityRectangleShape);

        static void resolveLightCollisions(const Entity& entity, Component::CShape& otherEntityRectangleShape);

        static void checkForLightIntersectWithShape(Component::CShape& otherEntityShape,
                Component::CLightSource& lightSource, Crucible::Ray& ray, size_t lineIndex);

        static void checkForLightIntersectWithWindowBorderSide(Component::CLightSource& lightSource, Crucible::Ray& ray,
                size_t lineIndex, Crucible::Vec2 windowBorderVertexA, Crucible::Vec2 windowBorderVertexB);

        static sf::Vector3f getManifold(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal);

        static void applyCollisionManifoldToTransform(Component::CCollidable& entityCollider,
                const sf::FloatRect& overlap, const Crucible::Vec2& result);

        static float crossProduct(Crucible::Vec2 a, Crucible::Vec2 b);

        static Crucible::LightRayIntersect
        isLineIntersecting(Crucible::Vec2 vertexA, Crucible::Vec2 vertexB, Crucible::Vec2 vertexC,
                Crucible::Vec2 vertexD);

    private:
        EntityManager& m_entityManager;
};

#endif //CRUCIBLE_COLLISION_SYSTEM_H