#pragma once

#ifndef CRUCIBLE_LIGHT_COLLISION_SYSTEM_H
#define CRUCIBLE_LIGHT_COLLISION_SYSTEM_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
#include <SFML/System/Vector3.hpp>

#include "core/manager/level_manager.h"
#include "system.h"
#include "entity_manager.h"
#include "core/engine/common_constants.h"

class LightCollisionSystem : public System
{
    public:
        explicit LightCollisionSystem(EntityManager& entityManager);

        void execute() override;

    private:
        void resolveLightCollisions(const Entity& entity);

        void checkForLightIntersectWithObject(
                Component::CLightSource& lightSource,
                const std::vector<Object>& lightObjects,
                Crucible::Ray& ray,
                size_t rayIndex,
                Crucible::RayType rayType);

        static float crossProduct(Crucible::Vec2 a, Crucible::Vec2 b);

        Crucible::LightRayIntersect isLineIntersecting(Crucible::Vec2 vertexA,
                Crucible::Vec2 vertexB,
                Crucible::Vec2 vertexC,
                Crucible::Vec2 vertexD,
                Crucible::EntityType entityType);

        void checkForLightIntersectForRay(
                Component::CLightSource& lightSource,
                size_t rayIndex,
                Crucible::RayType rayType,
                const Object& object,
                const Crucible::Vec2& rayStartPos,
                const Crucible::Vec2& rayEndPos,
                const sf::Vertex& objectStartVert,
                const sf::Vertex& objectEndVert);

        void resolveLightCollisionsForRayGroup(
                Component::CLightSource& lightSource,
                std::vector<Entity>& players,
                Crucible::RayType rayType);

    private:
        EntityManager& m_entityManager;
};

#endif //CRUCIBLE_LIGHT_COLLISION_SYSTEM_H