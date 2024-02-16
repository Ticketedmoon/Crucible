#pragma once

#ifndef CRUCIBLE_ANIMATION_SYSTEM_H
#define CRUCIBLE_ANIMATION_SYSTEM_H

#include <cassert>

#include <memory>
#include "system.h"
#include "tile.h"
#include "entity_manager.h"

class AnimationSystem : public System
{
    public:
        explicit AnimationSystem(EntityManager& entityManager);

        void execute() override;

    private:

        /**
         * Rotation outcomes:
         * None: [top-left vertex -> bottom-left vertex, clockwise]
         * 90 degree anti-clockwise texture rotation: [top-right vertex -> top-left vertex, clockwise]
         * 90 degree clockwise texture rotation: [bottom-left vertex -> bottom-right vertex, clockwise]
         * flip horizontally: [top-right -> bottom-right, anti-clockwise]
         */
         void updateTileTexture(
                Tile& tile,
                const std::shared_ptr<sf::Texture>& tileSheetTexture,
                const Component::CTransform& transform,
                Component::CAnimation& animation);

        static int updateAnimation(Tile& tile, Component::CAnimation& animation) ;

    private:
        EntityManager& m_entityManager;
};


#endif //CRUCIBLE_ANIMATION_SYSTEM_H
