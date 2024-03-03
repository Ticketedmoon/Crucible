#pragma once

#ifndef CRUCIBLE_TRANSFORM_SYSTEM_H
#define CRUCIBLE_TRANSFORM_SYSTEM_H

#include <cmath>
#include <cassert>
#include <cmath>
#include <unordered_set>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "system.h"
#include "entity/entity_manager.h"
#include "scene/gameplay_scene/system/render_system.h"

class TransformSystem : public System
{
    public:
        explicit TransformSystem(EntityManager& entityManager, sf::Clock& gameClock);

        void execute() override;

    private:
        static void resolveControllerMovementForEntity(const Entity& e, Component::CTransform& cTransform);
        static float distance(Crucible::Vec2 p1, Crucible::Vec2 p2);
        void moveToNextWaypoint(
                Component::CTransform& entityTransform,
                Component::CPathFollower& cPathFollower,
                Component::CAnimation& cAnimation) const;
        static void handleIdleAnimation(Component::CAnimation& animation);
        static void handleControllableEntityMovement(Component::CTransform& cTransform, const Component::CControllable& controllable);
        static void handleMovementAnimation(const Component::CControllable& controllable, Component::CAnimation& animation);
        static void moveToTargetPosition(
                Component::CTransform& entityTransform,
                const Crucible::Vec2& targetPosition,
                float entitySpeed);

    private:
        static inline const float PLAYER_SPEED = 3.0f;
        static inline const float GUARD_SPEED = 1.6f;
        static inline const float DIAGONAL_MOVEMENT_SPEED_OFFSET = 0.65f;

        EntityManager& m_entityManager;
        sf::Clock& m_gameClock;
};


#endif //CRUCIBLE_TRANSFORM_SYSTEM_H
