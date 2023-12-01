#pragma once

#ifndef CRUCIBLE_TRANSFORM_SYSTEM_H
#define CRUCIBLE_TRANSFORM_SYSTEM_H

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
        explicit TransformSystem(EntityManager& entityManager);

        void execute() override;

    private:
        static bool resolveControllerMovementForEntity(const Entity& e, Component::CTransform& cTransform);
        void updateVertexPositionsForEntity(const Component::CTransform& entityTransform,
                Component::CShape& entityRectangleShape) const;

    private:
        EntityManager& m_entityManager;
};


#endif //CRUCIBLE_TRANSFORM_SYSTEM_H
