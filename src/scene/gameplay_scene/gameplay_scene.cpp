#include "gameplay_scene.h"

GameplayScene::GameplayScene(GameEngine& engine) : Scene(engine)
{
    m_renderTexture.create(Crucible::WINDOW_WIDTH, Crucible::WINDOW_HEIGHT);
    m_renderSprite.setTexture(m_renderTexture.getTexture());
    m_renderSprite.setTextureRect(sf::IntRect(0, 0, Crucible::WINDOW_WIDTH, Crucible::WINDOW_HEIGHT));

    registerActions();
    registerSystems();
}

void GameplayScene::update()
{
    m_entityManager.update();
    m_systemManager.update();
}

void GameplayScene::render()
{
    gameEngine.window.clear();
    m_renderTexture.clear(LEVEL_BACKGROUND_COLOR);

    m_systemManager.render();

    m_renderTexture.display();
    m_renderSprite.setTexture(m_renderTexture.getTexture());
    gameEngine.window.draw(m_renderSprite, sf::RenderStates(sf::BlendAdd));

    gameEngine.window.display();
}

void GameplayScene::performAction(Action& action)
{
    if (Action::Mode::RELEASE == action.getMode())
    {
        return;
    }

    if (Action::Type::SCENE_EXIT == action.getType())
    {
        gameEngine.window.close();
    }
}

void GameplayScene::registerActions()
{
    registerActionType(sf::Keyboard::Key::Escape, Action::Type::SCENE_EXIT);
}

void GameplayScene::registerSystems()
{
    m_systemManager.registerSystem(
            std::make_shared<RenderSystem>(m_renderTexture, m_entityManager), SystemManager::SystemType::RENDER);
}