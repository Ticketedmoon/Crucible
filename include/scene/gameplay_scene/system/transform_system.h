#pragma once

#ifndef CRUCIBLE_TRANSFORM_SYSTEM_H
#define CRUCIBLE_TRANSFORM_SYSTEM_H

#include <cmath>
#include <cassert>
#include <cmath>

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
        explicit TransformSystem(EntityManager& entityManager);

        void execute() override;

    private:
        static void resolveControllerMovementForEntity(const Entity& e, Component::CTransform& cTransform);
        static float distance(Crucible::Vec2 p1, Crucible::Vec2 p2);

    private:
        static inline const float PLAYER_SPEED = 1.65f;
        static inline const float GUARD_SPEED = 1.05f;

        EntityManager& m_entityManager;
};


#endif //CRUCIBLE_TRANSFORM_SYSTEM_H
