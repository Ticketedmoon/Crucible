#pragma once

#ifndef CRUCIBLE_TILE_H
#define CRUCIBLE_TILE_H

#include <SFML/System/Vector2.hpp>
#include <cstdint>

#include <SFML/Graphics/VertexArray.hpp>
#include <utility>
#include <memory>

enum class TileType
{
    TRANSPARENT = 0,
    BEDROCK_BLOCK = 1,
    PEACH_CHARM_BLOCK = 2,
    ARROW_BLOCK = 3,
    QUESTION_BLOCK = 4,
    FACE_SMILE_BLOCK = 5,
    FACE_HAPPY_BLOCK = 6,
    FACE_UNHAPPY_BLOCK = 7,
    FACE_LOVE_BLOCK = 8,
    GREY_MIXED_BLOCK = 9,
    GREY_TAPE_BLOCK = 10,
    GREY_NOSE_BLOCK = 11,
    GREY_PLUS_BLOCK = 12,
    EXPLANATION_COLOUR_A_BLOCK = 13,
    EXPLANATION_COLOUR_B_BLOCK = 14,
    EXPLANATION_COLOUR_C_BLOCK = 15,
    EXPLANATION_COLOUR_D_BLOCK = 16,
    WHITE_CLOCK_BLOCK = 17,
    BLACK_DOT_BLOCK = 18,
    BROWN_FLOOR_BLOCK = 19,
    PURPLE_HEART_BLOCK = 20,
    ORANGE_PLUS_BLOCK = 21,
    PINK_BROOM_BLOCK = 22,
    SKY_SPAWN_BLOCK = 23,
    GREEN_GOAL_ZONE_BLOCK = 24,
    YELLOW_WRENCH_BLOCK = 25
};

class Tile
{
    public:
        Tile();
        Tile(sf::Vector2u position, TileType type, std::shared_ptr<sf::VertexArray> vertices);

        sf::Vector2u position;
        TileType type;
        std::shared_ptr<sf::VertexArray> vertices;
};

#endif //CRUCIBLE_TILE_H
