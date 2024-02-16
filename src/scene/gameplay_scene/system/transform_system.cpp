#include "transform_system.h"

TransformSystem::TransformSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{
}

void TransformSystem::execute()
{
    std::vector<Entity> entities = m_entityManager.getEntities();
    for (const Entity& entity : entities)
    {
        auto& entityTransform = entity.getComponent<Component::CTransform>();

        // @Refactor: Should we do this elsewhere?
        if (entity.hasComponent<Component::CControllable>())
        {
            resolveControllerMovementForEntity(entity, entityTransform);
        }

        if (entity.hasComponent<Component::CPathFollower>())
        {
            auto& cPathFollower = entity.getComponent<Component::CPathFollower>();

            // If at point, move to next destination
            Crucible::Vec2 waypoint = cPathFollower.path[cPathFollower.destinationIndex];
            float distanceToWaypoint = distance(waypoint, *entityTransform.position);
            if (distanceToWaypoint < 1)
            {
                cPathFollower.destinationIndex++;
            }

            // If destinationIndex is == path.size(), then reset to 0.
            if (cPathFollower.destinationIndex == cPathFollower.path.size())
            {
                cPathFollower.destinationIndex = 0;
                waypoint = cPathFollower.path[cPathFollower.destinationIndex];
            }

            // move forward (into the waypoint, based on the current object rotation)
            float r = std::atan2(waypoint.y - entityTransform.position->y, waypoint.x - entityTransform.position->x);
            entityTransform.position->x += std::cos(r) * GUARD_SPEED;
            entityTransform.position->y += std::sin(r) * GUARD_SPEED;
        }
    }
}

float TransformSystem::distance(Crucible::Vec2 p1, Crucible::Vec2 p2) {
    float xb = p2.x - p1.x;
    float yb = p2.y - p1.y;
    return std::sqrt((xb * xb) + (yb * yb));
}

void TransformSystem::resolveControllerMovementForEntity(const Entity& e, Component::CTransform& cTransform)
{
    auto& controllable = e.getComponent<Component::CControllable>();
    auto& animation = e.getComponent<Component::CAnimation>();

    // TODO @Refactor
    animation.animationList = {};

    if (controllable.isMovingDown)
    {
        cTransform.position->y += PLAYER_SPEED;
        animation.animationList = {
                TileType::PLAYER_WALK_DOWN_A, TileType::PLAYER_WALK_DOWN_B, TileType::PLAYER_WALK_DOWN_C,
                TileType::PLAYER_WALK_DOWN_D, TileType::PLAYER_WALK_DOWN_E, TileType::PLAYER_WALK_DOWN_F,
        };
    }
    if (controllable.isMovingUp)
    {
        cTransform.position->y -= PLAYER_SPEED;
        animation.animationList = {
                TileType::PLAYER_WALK_UP_A, TileType::PLAYER_WALK_UP_B, TileType::PLAYER_WALK_UP_C,
                TileType::PLAYER_WALK_UP_D, TileType::PLAYER_WALK_UP_E, TileType::PLAYER_WALK_UP_F,
                };
    }
    if (controllable.isMovingLeft)
    {
        cTransform.position->x -= PLAYER_SPEED;
        animation.animationList = {
                TileType::PLAYER_WALK_LEFT_A, TileType::PLAYER_WALK_LEFT_B, TileType::PLAYER_WALK_LEFT_C,
                TileType::PLAYER_WALK_LEFT_D, TileType::PLAYER_WALK_LEFT_E, TileType::PLAYER_WALK_LEFT_F
        };
    }
    if (controllable.isMovingRight)
    {
        cTransform.position->x += PLAYER_SPEED;
        animation.animationList = {
                TileType::PLAYER_WALK_RIGHT_A, TileType::PLAYER_WALK_RIGHT_B, TileType::PLAYER_WALK_RIGHT_C,
                TileType::PLAYER_WALK_RIGHT_D, TileType::PLAYER_WALK_RIGHT_E, TileType::PLAYER_WALK_RIGHT_F,
        };
    }
}