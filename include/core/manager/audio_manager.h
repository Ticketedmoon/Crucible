#pragma once

#ifndef CRUCIBLE_AUDIO_MANAGER_H
#define CRUCIBLE_AUDIO_MANAGER_H

#include <SFML/Audio.hpp>

#include <cassert>

#include "core/engine/action.h"

class AudioManager {
    public:
        enum class AudioType
        {
        };

        ~AudioManager();
        AudioManager(AudioManager& audioManager) = delete;
        void operator=(const AudioManager&) = delete;

        static AudioManager* getInstance();
        static void playSound(AudioType audioType, float volume);
        static void playMusic(uint8_t sceneIndex, float volume, bool shouldLoop);
        static bool isMusicPlaying();
        static void stopActiveMusic();

    private:
        AudioManager();

        static void loadSfx(AudioType audioType, const std::string& sfxPath);

    private:
        static inline AudioManager* m_audioManager;

        static inline std::unordered_map<AudioType, sf::SoundBuffer> m_soundBufferMap;
        static inline std::unordered_map<AudioType, sf::Sound> m_soundMap;
        static inline std::vector<sf::Music*> m_sceneMusic;
        static inline uint8_t currentSceneIndex;
};


#endif //CRUCIBLE_AUDIO_MANAGER_H
