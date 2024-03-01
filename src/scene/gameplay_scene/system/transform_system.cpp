#include "transform_system.h"

TransformSystem::TransformSystem(EntityManager& entityManager, sf::Clock& gameClock)
    : m_entityManager(entityManager), m_gameClock(gameClock)
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

            if (m_gameClock.getElapsedTime().asMilliseconds() < cPathFollower.gameTimeTicker.timeUntilUpdate)
            {
                continue;
            }

            moveToNextWaypoint(entityTransform, cPathFollower);
        }
    }
}

void TransformSystem::moveToNextWaypoint(
        Component::CTransform& entityTransform,
        Component::CPathFollower& cPathFollower) const
{
    Waypoint waypoint = cPathFollower.path[cPathFollower.destinationIndex];
    float distanceToWaypoint = distance(waypoint.position, *entityTransform.position);

    if (distanceToWaypoint < 1)
    {
        cPathFollower.gameTimeTicker.timeUntilUpdate = m_gameClock.getElapsedTime().asMilliseconds() + waypoint.waitPeriodMs;
        cPathFollower.destinationIndex++;
    }

    // If destinationIndex is == path.size(), then reset to 0.
    if (cPathFollower.destinationIndex == cPathFollower.path.size())
    {
        cPathFollower.destinationIndex = 0;
        waypoint = cPathFollower.path[cPathFollower.destinationIndex];
    }

    // move forward (into the waypoint, based on the current object rotation)
    float r = std::atan2(waypoint.position.y - entityTransform.position->y, waypoint.position.x - entityTransform.position->x);
    entityTransform.position->x += std::cos(r) * GUARD_SPEED;
    entityTransform.position->y += std::sin(r) * GUARD_SPEED;
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
    bool isMoving = controllable.isMovingDown || controllable.isMovingUp || controllable.isMovingRight || controllable.isMovingLeft;

    if (controllable.isMovingDown)
    {
        animation.entityDirection = EntityDirection::DOWN;
        cTransform.position->y += PLAYER_SPEED;
        animation.currentAnimation = EntityAnimation::PLAYER_WALK_DOWN;
    }
    if (controllable.isMovingUp)
    {
        animation.entityDirection = EntityDirection::UP;
        cTransform.position->y -= PLAYER_SPEED;
        animation.currentAnimation = EntityAnimation::PLAYER_WALK_UP;
    }
    if (controllable.isMovingLeft)
    {
        animation.entityDirection = EntityDirection::LEFT;
        cTransform.position->x -= PLAYER_SPEED;
        animation.currentAnimation = EntityAnimation::PLAYER_WALK_LEFT;
    }
    if (controllable.isMovingRight)
    {
        animation.entityDirection = EntityDirection::RIGHT;
        cTransform.position->x += PLAYER_SPEED;
        animation.currentAnimation = EntityAnimation::PLAYER_WALK_RIGHT;
    }

    if (!isMoving)
    {
        std::unordered_set<EntityAnimation> idleAnimations{
            EntityAnimation::PLAYER_IDLE_DOWN, EntityAnimation::PLAYER_IDLE_UP,
            EntityAnimation::PLAYER_IDLE_RIGHT, EntityAnimation::PLAYER_IDLE_LEFT};

        if (animation.currentAnimationFrameIdx > 0 && !idleAnimations.contains(animation.currentAnimation))
        {
            animation.currentAnimationFrameIdx = 0;
        }

        animation.currentAnimation = animation.entityDirection == EntityDirection::DOWN
                ? EntityAnimation::PLAYER_IDLE_DOWN
                : animation.entityDirection == EntityDirection::UP
                        ? EntityAnimation::PLAYER_IDLE_UP
                        : animation.entityDirection == EntityDirection::RIGHT
                                ? EntityAnimation::PLAYER_IDLE_RIGHT
                                : EntityAnimation::PLAYER_IDLE_LEFT;
    }
}