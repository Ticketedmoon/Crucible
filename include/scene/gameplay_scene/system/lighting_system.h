#pragma once

#ifndef CRUCIBLE_LIGHTING_SYSTEM_H
#define CRUCIBLE_LIGHTING_SYSTEM_H

#include <SFML/Graphics/RenderTarget.hpp>
#include "system.h"
#include "entity_manager.h"

class LightingSystem : public System
{
    public:
        explicit LightingSystem(EntityManager& entityManager);

        void execute() override;

    private:
        static std::vector<Crucible::LightRayIntersect> findAllRayIntersectionPoints(Component::CLightSource& entityLightSource,
                const Component::CTransform& entityTransform) ;
        static void addVerticesForLightCollisions(Component::CLightSource& entityLightSource,
                const Component::CTransform& entityTransform, const std::vector<Crucible::LightRayIntersect>& intersections) ;
        static Crucible::LightRayIntersect findClosestIntersectForLine(const Component::CTransform& entityTransform,
                std::vector<Crucible::LightRayIntersect>& intersectList) ;

    private:
        EntityManager& m_entityManager;
};


#endif //CRUCIBLE_LIGHTING_SYSTEM_H
