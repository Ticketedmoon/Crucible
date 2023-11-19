#pragma once

#ifndef CRUCIBLE_COMPONENT_H
#define CRUCIBLE_COMPONENT_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <utility>
#include "vec2.h"

namespace Component
{
    struct CTransform
    {
        Vec2 position;
    };
};

#endif //CRUCIBLE_COMPONENT_H
