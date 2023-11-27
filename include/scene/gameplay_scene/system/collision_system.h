#pragma once

#ifndef CRUCIBLE_COLLISION_SYSTEM_H
#define CRUCIBLE_COLLISION_SYSTEM_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
#include <SFML/System/Vector3.hpp>

#include "system.h"
#include "entity_manager.h"

class CollisionSystem : public System
{
    public:
        explicit CollisionSystem(EntityManager& entityManager);

        void execute() override;

    private:
        static bool isCollidingAABB(const Component::CShape& entityRect,
                const Component::CShape& otherEntityRect, sf::FloatRect& overlap);
        void resolveCollision(const Component::CShape& entityRectangleShape,
                Component::CTransform& entityTransform, const Component::CShape& otherEntityRectangleShape,
                const sf::FloatRect& overlap);
        sf::Vector3f getManifold(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal);

    private:
        EntityManager& m_entityManager;
};

#endif //CRUCIBLE_COLLISION_SYSTEM_H