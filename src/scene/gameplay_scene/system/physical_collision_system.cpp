#include "physical_collision_system.h"

PhysicalCollisionSystem::PhysicalCollisionSystem(
        EntityManager& entityManager,
        Crucible::GameProperties& gameProperties)
        : m_entityManager(entityManager), m_gameProperties(gameProperties)
{

}

void PhysicalCollisionSystem::execute()
{
    std::vector<Entity> entities = m_entityManager.getEntities();

    for (Entity& entity: entities)
    {
        if (entity.hasComponent<Component::CCollider>())
        {
            auto& entityCollider = entity.getComponent<Component::CCollider>();
            checkForLevelObjectLayerCollisions(entity, entityCollider);
            checkForOtherCollidableEntities(entities, entity);
        }

        // TODO Consider moving me elsewhere
        auto& entityTile = entity.getComponent<Component::CTile>();
        auto& entityTransform = entity.getComponent<Component::CTransform>();
        updateShapeVertexPositions(entityTransform, entityTile);
    }
}

void PhysicalCollisionSystem::resolvePhysicalCollisions(Entity& entity, Entity& otherEntity)
{
    auto& entityTile = entity.getComponent<Component::CTile>();
    auto& entityTransform = entity.getComponent<Component::CTransform>();
    auto& entityCollider = entity.getComponent<Component::CCollider>();
    auto& otherEntityTile = otherEntity.getComponent<Component::CTile>();
    auto& otherEntityTransform = otherEntity.getComponent<Component::CTransform>();
    auto& otherEntityCollider = otherEntity.getComponent<Component::CCollider>();

    sf::FloatRect overlap;
    if (!isCollidingAABB(entityTile, otherEntityTile.tile.vertices->getBounds(), overlap))
    {
        return;
    }

    // if entityA can collide with projectiles, and collision with entity B (projectile) induces a kill state
    if (entityCollider.collidableEntities.contains(Crucible::EntityType::PROJECTILE)
            && otherEntityCollider.shouldKill)
    {
        // destroy player
        entity.destroy();
        // destroy projectile
        otherEntity.destroy();
        return;
    }
    resolveCollisionByManifold(entityTile, entityTransform, *otherEntityTransform.position, overlap);
}

void PhysicalCollisionSystem::checkForOtherCollidableEntities(std::vector<Entity>& entities, Entity& entity)
{
    for (Entity& otherEntity: entities)
    {
        if (entity.getId() == otherEntity.getId())
        {
            // @Refactor: Can we do this a better way?
            continue;
        }

        if (otherEntity.hasComponent<Component::CCollider>())
        {
            resolvePhysicalCollisions(entity, otherEntity);
        }
    }
}
void PhysicalCollisionSystem::checkForLevelObjectLayerCollisions(const Entity& entity, Component::CCollider& entityCollider)
{
    if (!entityCollider.collidableEntities.contains(Crucible::EntityType::LEVEL_OBJECT))
    {
        return;
    }

    ObjectLayer& collisionLayerForPlayerOne
        = LevelManager::activeLevel.layerNameToObjectLayer.at(LevelManager::COLLISION_LAYER_PLAYER_A);
//    ObjectLayer lightingObjectLayerB
//            = LevelManager::activeLevel.layerNameToObjectLayer.at(LevelManager::COLLISION_LAYER_PLAYER_B);

    resolvePhysicalCollisionsForObjectLayer(entity, collisionLayerForPlayerOne);
    //resolvePhysicalCollisionsForObjectLayer(entityCollider, entity, lightingObjectLayerB);
}

void PhysicalCollisionSystem::resolvePhysicalCollisionsForObjectLayer(const Entity& entity, ObjectLayer& objectLayer)
{
    Component::CTile entityTile = entity.getComponent<Component::CTile>();
    Component::CTransform entityTransform = entity.getComponent<Component::CTransform>();

    for (const Object& object: objectLayer.objectData)
    {
        const std::shared_ptr<sf::VertexArray>& vArr = object.objectVertices;

        // TODO find a better abstraction for this logic
        if (object.objectName == "end zone")
        {
            // check if player collides with
            sf::FloatRect overlap;
            if (entity.hasComponent<Component::CControllable>()
                    && isCollidingAABB(entityTile, vArr->getBounds(), overlap))
            {
                m_gameProperties.isLevelCompleted = true;
            }
        }

        sf::Vertex v = (*vArr)[0];

        // TODO Centralise this block for collision manifold resolution
        sf::FloatRect overlap;
        if (isCollidingAABB(entityTile, vArr->getBounds(), overlap))
        {
            resolveCollisionByManifold(entityTile, entityTransform, {v.position.x, v.position.y}, overlap);
        }
    }
}

bool PhysicalCollisionSystem::isCollidingAABB(const Component::CTile& entityTile,
        const sf::FloatRect& otherObjectBounds, sf::FloatRect& overlap)
{
    return entityTile.tile.vertices->getBounds().intersects(otherObjectBounds, overlap);
}

void PhysicalCollisionSystem::applyCollisionOverlapToEntityTransform(Component::CTransform& entityTransform,
        sf::Vector3f manifoldDist, sf::Vector2f collisionOverlap)
{
    if (manifoldDist.y == 1)
    {
        // Bottom Collision
        entityTransform.position->y -= collisionOverlap.y;
    }
    if (manifoldDist.y == -1)
    {
        // Top Collision
        entityTransform.position->y += collisionOverlap.y;
    }

    if (manifoldDist.x == 1)
    {
        // Left Collision
        entityTransform.position->x -= collisionOverlap.x;
    }
    if (manifoldDist.x == -1)
    {
        // Right Collision
        entityTransform.position->x += collisionOverlap.x;
    }
}

void PhysicalCollisionSystem::updateShapeVertexPositions(const Component::CTransform& entityTransform,
        Component::CTile& entityTile)
{
    // Update rect based on transform points
    sf::VertexArray& tileVertices = *entityTile.tile.vertices;
    tileVertices[0].position = {entityTransform.position->x, entityTransform.position->y};
    tileVertices[1].position = {entityTransform.position->x + entityTransform.dimensions.x, entityTransform.position->y};
    tileVertices[2].position = {entityTransform.position->x + entityTransform.dimensions.x,
                                entityTransform.position->y + entityTransform.dimensions.y };
    tileVertices[3].position = {entityTransform.position->x, entityTransform.position->y + entityTransform.dimensions.y };
    tileVertices[4].position = {entityTransform.position->x, entityTransform.position->y};
}

void PhysicalCollisionSystem::resolveCollisionByManifold(
        Component::CTile& entityTile,
        Component::CTransform& entityTransform,
        const Crucible::Vec2& otherEntityPositionVec,
        const sf::FloatRect& overlap)
{
    sf::VertexArray& tileVertices = *entityTile.tile.vertices;
    float xDiff = std::abs(tileVertices[0].position.x - tileVertices[1].position.x) / 2;
    float yDiff = std::abs(tileVertices[0].position.y - tileVertices[2].position.y) / 2;

    float oxDiff = std::abs(tileVertices[0].position.x - tileVertices[1].position.x) / 2;
    float oyDiff = std::abs(tileVertices[0].position.y - tileVertices[2].position.y) / 2;

    const Crucible::Vec2& entityPosition = Crucible::Vec2(entityTransform.position->x - xDiff, entityTransform.position->y + yDiff);
    const Crucible::Vec2& otherEntityPosition = Crucible::Vec2(otherEntityPositionVec.x - oxDiff, otherEntityPositionVec.y + oyDiff);
    const Crucible::Vec2& result = entityPosition - otherEntityPosition;

    applyCollisionManifoldToTransform(entityTransform, overlap, result);
}

void PhysicalCollisionSystem::applyCollisionManifoldToTransform(Component::CTransform& cTransform,
        const sf::FloatRect& overlap, const Crucible::Vec2& result)
{
    sf::Vector2f collisionNormal{result.x, result.y};
    sf::Vector3f manifoldDist = getManifold(overlap, collisionNormal);
    sf::Vector2f collisionOverlap{overlap.width, overlap.height};

    applyCollisionOverlapToEntityTransform(cTransform, manifoldDist, collisionOverlap);
}

sf::Vector3f PhysicalCollisionSystem::getManifold(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal)
{
    // the collision normal is stored in x and y, with the penetration in z
    sf::Vector3f manifold;

    if (overlap.width < overlap.height)
    {
        manifold.x = (collisionNormal.x < 0) ? 1.f : -1.f;
        manifold.z = overlap.width;
    }
    else
    {
        manifold.y = (collisionNormal.y < 0) ? 1.f : -1.f;
        manifold.z = overlap.height;
    }

    return manifold;
}