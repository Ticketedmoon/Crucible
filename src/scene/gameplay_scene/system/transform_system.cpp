#include <unordered_set>
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

    moveToTargetPosition(entityTransform, waypoint.position, GUARD_SPEED);
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
    if (isMoving)
    {
        handleControllableEntityMovement(cTransform, controllable);
        handleMovementAnimation(controllable, animation);
        return;
    }

    handleIdleAnimation(animation);
}

void TransformSystem::handleControllableEntityMovement(
        Component::CTransform& cTransform,
        const Component::CControllable& controllable)
{
    Crucible::Vec2 newPos = *cTransform.position;
    newPos.y += controllable.isMovingDown ? PLAYER_SPEED : 0;
    newPos.y -= controllable.isMovingUp ? PLAYER_SPEED : 0;
    newPos.x += controllable.isMovingRight ? PLAYER_SPEED : 0;
    newPos.x -= controllable.isMovingLeft ? PLAYER_SPEED : 0;

    moveToTargetPosition(cTransform, newPos, PLAYER_SPEED);
}

void TransformSystem::moveToTargetPosition(
        Component::CTransform& entityTransform,
        const Crucible::Vec2& targetPosition,
        const float entitySpeed)
{
    float r = std::atan2(targetPosition.y - entityTransform.position->y, targetPosition.x - entityTransform.position->x);
    entityTransform.position->x += std::cos(r) * entitySpeed;
    entityTransform.position->y += std::sin(r) * entitySpeed;
}

void TransformSystem::handleMovementAnimation(const Component::CControllable& controllable,
        Component::CAnimation& animation)
{
    animation.entityDirection =
            controllable.isMovingDown ? EntityDirection::DOWN
            : controllable.isMovingUp ? EntityDirection::UP
            : controllable.isMovingRight ? EntityDirection::RIGHT : EntityDirection::LEFT;
    animation.currentAnimation =
            controllable.isMovingDown ? EntityAnimation::PLAYER_WALK_DOWN
            : controllable.isMovingUp ? EntityAnimation::PLAYER_WALK_UP
            : controllable.isMovingRight ? EntityAnimation::PLAYER_WALK_RIGHT : EntityAnimation::PLAYER_WALK_LEFT;
}

void TransformSystem::handleIdleAnimation(Component::CAnimation& animation)
{
    std::unordered_set<EntityAnimation> idleAnimations{
            EntityAnimation::PLAYER_IDLE_DOWN, EntityAnimation::PLAYER_IDLE_UP,
            EntityAnimation::PLAYER_IDLE_RIGHT, EntityAnimation::PLAYER_IDLE_LEFT};

    animation.currentAnimationFrameIdx = animation.currentAnimationFrameIdx > 0 && !idleAnimations.contains(animation.currentAnimation)
            ? 0
            : animation.currentAnimationFrameIdx;

    animation.currentAnimation =
            animation.entityDirection == EntityDirection::DOWN ? EntityAnimation::PLAYER_IDLE_DOWN
            : animation.entityDirection == EntityDirection::UP ? EntityAnimation::PLAYER_IDLE_UP
            : animation.entityDirection == EntityDirection::RIGHT ? EntityAnimation::PLAYER_IDLE_RIGHT
            : EntityAnimation::PLAYER_IDLE_LEFT;
}
