#pragma once

#ifndef CRUCIBLE_GAMEPLAY_SCENE_H
#define CRUCIBLE_GAMEPLAY_SCENE_H

#include <SFML/Graphics.hpp>

#include "game_engine.h"
#include "scene.h"
#include "entity_manager.h"
#include "system_manager.h"
#include "render_system.h"

#include "texture_manager.h"

class GameplayScene : public Scene
{
    public:
        explicit GameplayScene(GameEngine& gameEngine);

        void update() override;
        void render() override;
        void performAction(Action& action) override;

    private:
        void registerSystems();
        void registerActions();

    private:
        static const inline sf::Color LEVEL_BACKGROUND_COLOR = sf::Color{0, 148, 32};

        EntityManager m_entityManager;
        SystemManager m_systemManager;

        sf::RenderTexture m_renderTexture;
        sf::Sprite m_renderSprite;
};


#endif //CRUCIBLE_GAMEPLAY_SCENE_H
