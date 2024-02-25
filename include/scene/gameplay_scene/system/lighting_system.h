#pragma once

#ifndef CRUCIBLE_LIGHTING_SYSTEM_H
#define CRUCIBLE_LIGHTING_SYSTEM_H

#include "victory_scene/victory_scene.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include "system.h"
#include "entity_manager.h"
#include "game_engine.h"

class LightingSystem : public System
{
    public:
        explicit LightingSystem(EntityManager& entityManager);

        void execute() override;

    private:
        std::vector<Crucible::LightRayIntersect> findAllRayIntersectionPoints(
                Crucible::RayType rayType,
                Component::CLightSource& entityLightSource,
                const Component::CTransform& entityTransform);
        void addVerticesForLightCollisions(Component::CLightSource& entityLightSource,
                const Component::CTransform& entityTransform, const std::vector<Crucible::LightRayIntersect>& intersections) ;
        void sortLightIntersectionsByDistanceToEntity(const Component::CTransform& entityTransform,
                std::vector<Crucible::LightRayIntersect>& intersectList);
        void sortIntersectionsByAngleAscending(const Component::CTransform& entityTransform,
                std::vector<Crucible::LightRayIntersect>& intersections);

    private:
        static inline const sf::Color& LIGHTING_COLOR{255, 255, 0};

        EntityManager& m_entityManager;
};


#endif //CRUCIBLE_LIGHTING_SYSTEM_H
