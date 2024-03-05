#pragma once

#ifndef CRUCIBLE_TEXTURE_MANAGER_H
#define CRUCIBLE_TEXTURE_MANAGER_H

#include <SFML/Graphics.hpp>

#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <filesystem>

class TextureManager
{
    public:
        TextureManager();
        std::shared_ptr<sf::Texture>& addTexture(const std::string& filePath);
        std::shared_ptr<sf::Texture>& getTexture(const std::string& filePath);
        bool hasTexture(const std::string& id);

    private:
        std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textureMap_;
};

#endif //CRUCIBLE_TEXTURE_MANAGER_H