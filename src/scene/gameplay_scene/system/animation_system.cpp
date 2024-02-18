#include "animation_system.h"

AnimationSystem::AnimationSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void AnimationSystem::execute()
{
    std::vector<Entity> entities = m_entityManager.getEntities();
    for (auto& entity: entities)
    {
        if (!entity.hasComponent<Component::CAnimation>())
        {
            continue;
        }

        auto& entityTransform = entity.getComponent<Component::CTransform>();
        auto& entityAnimation = entity.getComponent<Component::CAnimation>();
        auto& entityCTile = entity.getComponent<Component::CTile>();

        updateTileTexture(entityCTile.tile, entityCTile.texture, entityTransform, entityAnimation);
    }
}

void AnimationSystem::updateTileTexture(
        Tile& tile,
        const std::shared_ptr<sf::Texture>& tileSheetTexture,
        const Component::CTransform& transform,
        Component::CAnimation& animation)
{
    sf::VertexArray& tileVertices = *tile.vertices;
    assert(tileVertices.getVertexCount() == 4);

    int tileTypeValue = updateAnimation(tile, animation);

    // TODO the below logic only needs to be called once for static entities, we are doing additional work here unnecessarily.
    float tu = (tileTypeValue % (tileSheetTexture->getSize().x / static_cast<uint8_t>(transform.dimensions.x)));
    float tv = tileTypeValue / (tileSheetTexture->getSize().x / static_cast<uint8_t>(transform.dimensions.y));

    float tuPositionStart = tu * transform.dimensions.x;
    float tuPositionEnd = (tu + 1) * transform.dimensions.x;
    float tvPositionStart = tv * transform.dimensions.y;
    float tvPositionEnd = (tv + 1) * transform.dimensions.y;

    if (tile.rotation == TileRotation::NONE)
    {
        tileVertices[0].texCoords = {tuPositionStart, tvPositionStart};
        tileVertices[1].texCoords = {tuPositionEnd, tvPositionStart};
        tileVertices[2].texCoords = {tuPositionEnd, tvPositionEnd};
        tileVertices[3].texCoords = {tuPositionStart, tvPositionEnd};
        return;
    }

    if (tile.rotation == TileRotation::FLIPPED_HORIZONTALLY)
    {
        tileVertices[0].texCoords = sf::Vector2f(tuPositionEnd, tvPositionStart);
        tileVertices[1].texCoords = sf::Vector2f(tuPositionStart, tvPositionStart);
        tileVertices[2].texCoords = sf::Vector2f(tuPositionStart, tvPositionEnd);
        tileVertices[3].texCoords = sf::Vector2f(tuPositionEnd, tvPositionEnd);
        return;
    }
}

int AnimationSystem::updateAnimation(Tile& tile, Component::CAnimation& animation)
{
    int tileTypeValue = static_cast<int>(tile.tileIdx) - 1;
    if (animation.animationTileIdxList.empty())
    {
        return tileTypeValue;
    }

    tileTypeValue = static_cast<int>(animation.animationTileIdxList[animation.currentAnimationFrameIdx]) - 1;

    double& spriteAnimationTime = animation.animationTicker.timeUntilUpdate;
    double animationCompletionTime = animation.animationTicker.currentTime;
    spriteAnimationTime += Crucible::DT;

    if (spriteAnimationTime >= animationCompletionTime)
    {
        animation.currentAnimationFrameIdx++;
        if (animation.currentAnimationFrameIdx == animation.animationTileIdxList.size())
        {
            animation.currentAnimationFrameIdx = 0;
        }
        spriteAnimationTime = 0;
    }
    return tileTypeValue;
}
