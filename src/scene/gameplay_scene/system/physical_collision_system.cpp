#include "physical_collision_system.h"

PhysicalCollisionSystem::PhysicalCollisionSystem(GameEngine& gameEngine, EntityManager& entityManager)
    : m_gameEngine(gameEngine), m_entityManager(entityManager)
{

}

void PhysicalCollisionSystem::execute()
{
    std::vector<Entity> entities = m_entityManager.getEntities();

    for (const Entity& entity: entities)
    {
        if (entity.hasComponent<Component::CCollider>())
        {
            auto& entityCollider = entity.getComponent<Component::CCollider>();
            checkForLevelObjectLayerCollisions(entity, entityCollider);
            checkForOtherCollidableEntities(entities, entity, entityCollider);
        }

        // TODO Consider moving me elsewhere
        auto& entityTile = entity.getComponent<Component::CTile>();
        auto& entityTransform = entity.getComponent<Component::CTransform>();
        updateShapeVertexPositions(entityTransform, entityTile);
    }
}

void PhysicalCollisionSystem::resolvePhysicalCollisions(Component::CTile& entityTile,
        Component::CTransform& entityTransform, Component::CCollider entityCollider,
        const Crucible::Vec2& otherRectPos, const std::shared_ptr<sf::VertexArray>& otherTileVertices)
{
    if (entityCollider.immovable)
    {
        return;
    }

    sf::FloatRect overlap;
    if (isCollidingAABB(entityTile, otherTileVertices, overlap))
    {
        resolveCollision(entityTile, entityTransform, otherRectPos, overlap);
    }
}

void PhysicalCollisionSystem::checkForOtherCollidableEntities(std::vector<Entity>& entities,
        const Entity& entity, Component::CCollider& entityCollider)
{
    for (const Entity& otherEntity: entities)
    {
        if (entity.getId() == otherEntity.getId())
        {
            // @Refactor: Can we do this a better way?
            continue;
        }

        if (otherEntity.hasComponent<Component::CCollider>())
        {
            auto& entityTile = entity.getComponent<Component::CTile>();
            auto& entityTransform = entity.getComponent<Component::CTransform>();
            auto& otherEntityTile = otherEntity.getComponent<Component::CTile>();
            auto& otherEntityTransform = otherEntity.getComponent<Component::CTransform>();

            resolvePhysicalCollisions(entityTile,
                    entityTransform,
                    entityCollider,
                    *otherEntityTransform.position,
                    otherEntityTile.tile.vertices);
        }
    }
}
void PhysicalCollisionSystem::checkForLevelObjectLayerCollisions(const Entity& entity, Component::CCollider& entityCollider)
{
    ObjectLayer lightingObjectLayerA
        = LevelManager::activeLevel.layerNameToObjectLayer.at(LevelManager::LIGHTING_OBJECT_LAYER_A_NAME);
    ObjectLayer lightingObjectLayerB
            = LevelManager::activeLevel.layerNameToObjectLayer.at(LevelManager::LIGHTING_OBJECT_LAYER_B_NAME);

    resolvePhysicalCollisionsForObjectLayer(entityCollider, entity, lightingObjectLayerA);
    resolvePhysicalCollisionsForObjectLayer(entityCollider, entity, lightingObjectLayerB);
}

void PhysicalCollisionSystem::resolvePhysicalCollisionsForObjectLayer(Component::CCollider& entityCollider,
        const Entity& entity, ObjectLayer& lightingObjectLayer)
{
    for (const Object& object: lightingObjectLayer.lightingObjectData)
    {
        // FIXME: Problem here, we need to separate out the different tile sheets to enum values.
        //        It is jarring for the different enum key to share the same values and causes some difficult-to-debug issues.
        if (object.type == TileType::SPAWN_ZONE)
        {
            continue;
        }

        if (object.type == TileType::END_ZONE)
        {
            sf::FloatRect overlap;
            Component::CTile& entityTile = entity.getComponent<Component::CTile>();
            if (entity.hasComponent<Component::CControllable>() && isCollidingAABB(entityTile, object.objectVertices, overlap))
            {
                m_gameEngine.changeScene(Scene::Type::VICTORY_SCREEN, std::make_shared<VictoryScene>(m_gameEngine));
            }

            continue;
        }

        const std::shared_ptr<sf::VertexArray>& vArr = object.objectVertices;
        sf::Vertex v = (*vArr)[0];

        Component::CTile entityTile = entity.getComponent<Component::CTile>();
        Component::CTransform entityTransform = entity.getComponent<Component::CTransform>();
        resolvePhysicalCollisions(
                entityTile,
                entityTransform,
                entityCollider,
                {v.position.x, v.position.y},
                vArr);
    }
}

bool PhysicalCollisionSystem::isCollidingAABB(const Component::CTile& entityTile,
        const std::shared_ptr<sf::VertexArray>& otherRectVertices, sf::FloatRect& overlap)
{
    return entityTile.tile.vertices->getBounds().intersects(otherRectVertices->getBounds(), overlap);
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

void PhysicalCollisionSystem::resolveCollision(Component::CTile& entityTile, Component::CTransform& entityTransform,
        const Crucible::Vec2& otherEntityPositionVec, const sf::FloatRect& overlap)
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