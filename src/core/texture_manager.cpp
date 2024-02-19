#include "texture_manager.h"

TextureManager::TextureManager() = default;

void TextureManager::addTexture(const std::string& filePath) {
    auto it = textureMap_.find(filePath);
    if (it != textureMap_.end()) {
        std::string msg = "Unable to add texture: '" + filePath + "' already exists\n";
        throw std::runtime_error(msg);
    }

    std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile(filePath)) {
        std::string msg = "Unable to open texture '" + filePath + "'\n";
        throw std::runtime_error(msg);
    }

    textureMap_.emplace(filePath, texture);
    std::cout << "Texture with path: [" << filePath << "] has been added, size: [" << texture->getSize().x << ", "
        << texture->getSize().y << "]\n";
}

std::shared_ptr<sf::Texture>& TextureManager::getTexture(const std::string& filePath) {
    auto it = textureMap_.find(filePath);
    if (it == textureMap_.end()) {
        std::string msg = "Unable to add texture: '" + filePath + "' doesn't exist\n";
        throw std::runtime_error(msg);
    }

    return it->second;
}

bool TextureManager::hasTexture(const std::string& id)
{
    return textureMap_.contains(id);
}
