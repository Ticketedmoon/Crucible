#pragma once

#ifndef CRUCIBLE_COMMON_PROPERTIES_H
#define CRUCIBLE_COMMON_PROPERTIES_H

#include <cstdint>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

namespace Crucible
{
    struct GameProperties
    {
        bool isLevelCompleted{false};
    };
}


#endif //CRUCIBLE_COMMON_PROPERTIES_H
