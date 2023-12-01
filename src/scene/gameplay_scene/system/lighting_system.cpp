#include "lighting_system.h"

LightingSystem::LightingSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void LightingSystem::execute()
{
    // Add 'lightSourceSystem' or similar.
    // In the logic below, add all 'LightRayIntersect' objects that hit the line segment to list.
    // Sort the list based on closest to player position and take the first item.
    //   > - the player x, player y and take closet to 0
    // Append that items vertices value to the lightVertices array.


    std::vector<Entity> entities = m_entityManager.getEntities();
    for (const Entity entity : entities)
    {
        if (!entity.hasComponent<Component::CLightSource>() || !entity.hasComponent<Component::CTransform>())
        {
            continue;
        }

        auto& entityLightSource = entity.getComponent<Component::CLightSource>();
        if (entityLightSource.lightRayIntersects.empty())
        {
            continue;
        }

        // Sort intersect points based on closed to player (use Vec2 Distance formula)
        Component::CTransform cTransform = entity.getComponent<Component::CTransform>();

        // Note: This will only work for 1 hit, if we want multiple ray hits we'll need a more scalable solution.
        //       Add this later.
        Crucible::LightRayIntersect closestIntersect = entityLightSource.lightRayIntersects.at(0);
        double distToPlayer = cTransform.position.dist(entityLightSource.lightRayIntersects.at(0).pos);
        for (Crucible::LightRayIntersect intersect : entityLightSource.lightRayIntersects)
        {
            double nextDistToPlayer = cTransform.position.dist(intersect.pos);
            if (nextDistToPlayer < distToPlayer)
            {
                distToPlayer = nextDistToPlayer;
                closestIntersect = intersect;
            }
        }

        // Clear current vertices and intersects if present
        entityLightSource.lightRayIntersects.clear();
        entityLightSource.lightVertices.clear();

        // Add that item to lightVertices array.
        entityLightSource.lightVertices.append(sf::Vertex({cTransform.position.x, cTransform.position.y}, sf::Color::Red));
        entityLightSource.lightVertices.append(sf::Vertex({closestIntersect.pos.x, closestIntersect.pos.y}, sf::Color::Red));
    }
}
