#pragma once

#ifndef CRUCIBLE_RAY_APPENDER_SYSTEM_H
#define CRUCIBLE_RAY_APPENDER_SYSTEM_H

#include "system.h"
#include "entity_manager.h"
#include "level_manager.h"

/**
 * [System Description]
 * This system will add additional rays to the left and right of each vertex to give the visibility effect
 * a more-full look.
*/
class RayAppenderSystem  : public System
{
    public:
        explicit RayAppenderSystem(EntityManager& entityManager);

        void execute() override;

    private:
        static void updateAdditionalRayEndPosition(
                Component::CTransform& entityTransform,
                Crucible::LightRayGroup& rayGroup,
                size_t additionalRayGroupStartIdx,
                size_t rayIndex,
                const sf::Vertex& rayEndVert);

    private:
        EntityManager& m_entityManager;

    private:
        static const size_t RAY_DELTA = 1;
        static const size_t RAY_SCALE = 100;
        static constexpr float RADIANS_OFFSET = 0.3174533f;
        static constexpr size_t TOTAL_ADDITIONAL_RAYS_PER_VERT = 2;
};


#endif //CRUCIBLE_RAY_APPENDER_SYSTEM_H
