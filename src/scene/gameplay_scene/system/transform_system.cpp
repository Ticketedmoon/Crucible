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
            auto& cAnimation = entity.getComponent<Component::CAnimation>();
            if (m_gameClock.getElapsedTime().asMilliseconds() < cPathFollower.gameTimeTicker.timeUntilUpdate)
            {
                handleIdleAnimation(cAnimation);
                continue;
            }

            moveToNextWaypoint(entityTransform, cPathFollower, cAnimation);
        }

        if (entity.hasComponent<Component::CProjectile>())
        {
            auto& cProjectile  = entity.getComponent<Component::CProjectile>();
            *entityTransform.position += cProjectile.projectileDirectionVector;
        }
    }
}

bool TransformSystem::canExecute(const Crucible::GameProperties& gameProperties)
{
    return !gameProperties.isLevelCompleted;
}

void TransformSystem::moveToNextWaypoint(
        Component::CTransform& entityTransform,
        Component::CPathFollower& cPathFollower,
        Component::CAnimation& cAnimation) const
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

    // Determine direction
    float r = std::atan2(waypoint.position.y - entityTransform.position->y, waypoint.position.x - entityTransform.position->x);
    double deg = -r / M_PI * 180.f;
    deg += deg < 0.0f ? 360.0f : 0.0f;

    float DIRECTION_DEGREE_TOLERANCE = 30.0f;

    // TODO @Refactor
    //      We don't want to use CControllable here since this isn't a controllable entity.
    //      The degree values are hard-coded, we should refactor this code and improve maintainability.
    const Component::CControllable controllable{
        deg > 90-DIRECTION_DEGREE_TOLERANCE && deg < 90+DIRECTION_DEGREE_TOLERANCE,
        deg > 270-DIRECTION_DEGREE_TOLERANCE && deg < 270+DIRECTION_DEGREE_TOLERANCE,
        deg > 180-DIRECTION_DEGREE_TOLERANCE && deg < 180+DIRECTION_DEGREE_TOLERANCE,
        ((deg > 0-DIRECTION_DEGREE_TOLERANCE && deg < 0+DIRECTION_DEGREE_TOLERANCE)
        || (deg > 360-DIRECTION_DEGREE_TOLERANCE && deg < 360+DIRECTION_DEGREE_TOLERANCE))
    };

    handleMovementAnimation(controllable, cAnimation);
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
    float speed = PLAYER_SPEED;

    Crucible::Vec2 newPos = *cTransform.position;
    newPos.y += controllable.isMovingDown ? speed : 0;
    newPos.y -= controllable.isMovingUp ? speed : 0;
    newPos.x += controllable.isMovingRight ? speed : 0;
    newPos.x -= controllable.isMovingLeft ? speed : 0;

    bool isDiagonalDownwardMovement = controllable.isMovingDown && (controllable.isMovingLeft || controllable.isMovingRight);
    bool isDiagonalUpwardMovement = controllable.isMovingUp && (controllable.isMovingLeft || controllable.isMovingRight);

    if (isDiagonalDownwardMovement || isDiagonalUpwardMovement)
    {
        speed += DIAGONAL_MOVEMENT_SPEED_OFFSET;
    }

    moveToTargetPosition(cTransform, newPos, speed);
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
