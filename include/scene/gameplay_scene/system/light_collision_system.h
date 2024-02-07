#pragma once

#ifndef CRUCIBLE_LIGHT_COLLISION_SYSTEM_H
#define CRUCIBLE_LIGHT_COLLISION_SYSTEM_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
#include <SFML/System/Vector3.hpp>

#include "level_manager.h"
#include "system.h"
#include "entity_manager.h"
#include "common_constants.h"

class LightCollisionSystem : public System
{
    public:
        explicit LightCollisionSystem(EntityManager& entityManager);

        void execute() override;

    private:
        static void resolveLightCollisions(const Entity& entity);

        static void checkForLightIntersectWithObject(
                Component::CLightSource& lightSource, Crucible::Ray& ray, size_t lineIndex);

        static void checkForLightIntersectWithWindowBorderSide(Component::CLightSource& lightSource, Crucible::Ray& ray,
                size_t rayIndex, Crucible::Vec2 windowBorderVertexA, Crucible::Vec2 windowBorderVertexB);

        static float crossProduct(Crucible::Vec2 a, Crucible::Vec2 b);

        static Crucible::LightRayIntersect isLineIntersecting(Crucible::Vec2 vertexA, Crucible::Vec2 vertexB,
                Crucible::Vec2 vertexC, Crucible::Vec2 vertexD);

    private:
        EntityManager& m_entityManager;
};

#endif //CRUCIBLE_LIGHT_COLLISION_SYSTEM_H