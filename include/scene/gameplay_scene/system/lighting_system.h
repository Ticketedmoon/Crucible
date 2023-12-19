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
        void addVerticesForLightCollisions(Component::CLightSource& entityLightSource,
                const Component::CTransform& entityTransform) const;
        Crucible::LightRayIntersect findClosestIntersectForLine(const Component::CTransform& entityTransform,
                std::vector<Crucible::LightRayIntersect>& intersectList) const;

    private:
        EntityManager& m_entityManager;
};


#endif //CRUCIBLE_LIGHTING_SYSTEM_H
