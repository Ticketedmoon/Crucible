#pragma once

#ifndef CRUCIBLE_VERTEX_H
#define CRUCIBLE_VERTEX_H

#include <SFML/Graphics/Color.hpp>
#include "vec2.h"

namespace Crucible
{
    class Vertex
    {
        public:
            Vec2 position;
            sf::Color colour;
    };
}


#endif //CRUCIBLE_VERTEX_H
