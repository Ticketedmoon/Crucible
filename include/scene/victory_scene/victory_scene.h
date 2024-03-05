#pragma once

#ifndef CRUCIBLE_VICTORY_SCENE_H
#define CRUCIBLE_VICTORY_SCENE_H

#include "scene.h"

#include "victory_scene/system/victory_render_system.h"
#include "system_manager.h"

#include "game_engine.h"

class VictoryScene : public Scene
{
    public:
        explicit VictoryScene(GameEngine& gameEngine);

        void update() override;
        void render() override;
        void performAction(Action& action) override;

    private:
        void registerSystems();
        void registerActions();

    private:
        static const inline sf::Color LEVEL_BACKGROUND_COLOR = sf::Color{255, 255, 255};

        EntityManager m_entityManager;
        SystemManager m_systemManager;

        Crucible::GameProperties gameProperties;
};


#endif //CRUCIBLE_VICTORY_SCENE_H
