#pragma once

#ifndef CRUCIBLE_RENDER_SYSTEM_H
#define CRUCIBLE_RENDER_SYSTEM_H

#include <cmath>
#include <cassert>

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
        explicit TransformSystem(sf::RenderTarget& renderTarget, EntityManager& entityManager);

        void execute() override;

    private:
        sf::RenderTarget& m_renderTarget;
        EntityManager& m_entityManager;
};


#endif //CRUCIBLE_RENDER_SYSTEM_H
