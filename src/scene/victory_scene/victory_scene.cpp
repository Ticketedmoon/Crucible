#include "victory_scene/victory_scene.h"

VictoryScene::VictoryScene(GameEngine& engine) : Scene(engine)
{
    registerActions();
    registerSystems();
}

void VictoryScene::update()
{
    m_entityManager.update();
    m_systemManager.update();
}

void VictoryScene::render()
{
    gameEngine.m_renderTexture.clear(LEVEL_BACKGROUND_COLOR);

    m_systemManager.render();
    gameEngine.m_renderTexture.display();
    gameEngine.m_renderSprite.setTexture(gameEngine.m_renderTexture.getTexture());

    gameEngine.window.clear();
    gameEngine.window.draw(gameEngine.m_renderSprite);
    gameEngine.window.display();
}

void VictoryScene::performAction(Action& action)
{
    if (Action::Type::SCENE_EXIT == action.getType())
    {
        if (action.getMode() == Action::Mode::RELEASE)
        {
            return;
        }
        gameEngine.window.close();
    }
}

void VictoryScene::registerActions()
{
    // Escape
    registerActionType(sf::Keyboard::Key::Escape, Action::Type::SCENE_EXIT);
}

void VictoryScene::registerSystems()
{
    // Render
    m_systemManager.registerSystem(
            std::make_shared<VictoryRenderSystem>(gameEngine.m_renderTexture, m_entityManager), SystemManager::SystemType::RENDER);
}