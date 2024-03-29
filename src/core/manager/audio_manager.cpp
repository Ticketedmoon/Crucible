#include "core/manager/audio_manager.h"

AudioManager::AudioManager()
{
}

void AudioManager::loadSfx(const AudioType audioType, const std::string& sfxPath)
{
    assert(!m_soundBufferMap.contains(audioType));

    m_soundBufferMap[audioType] = sf::SoundBuffer();
    m_soundMap[audioType] = sf::Sound(m_soundBufferMap[audioType]);
    bool wasSfxLoaded = m_soundBufferMap[audioType].loadFromFile(sfxPath);

    assert(wasSfxLoaded);
}

AudioManager::~AudioManager()
{
    for (sf::Music* music : m_sceneMusic)
    {
        delete music;
    }

    delete m_audioManager;
    m_sceneMusic.clear();
}

AudioManager* AudioManager::getInstance()
{
    if (m_audioManager != nullptr)
    {
        return m_audioManager;
    }

    m_audioManager = new AudioManager();
    currentSceneIndex = 0;

    return m_audioManager;
}

void AudioManager::playSound(AudioType actionType, float volume)
{
    sf::Sound& sound = m_soundMap[actionType];
    sound.setVolume(volume);
    sound.play();
}

void AudioManager::playMusic(uint8_t sceneIndex, float volume, bool shouldLoop)
{
    stopActiveMusic();

    sf::Music& nextMusic = *m_sceneMusic[sceneIndex];
    nextMusic.setVolume(volume);
    nextMusic.setLoop(shouldLoop);
    nextMusic.play();

    currentSceneIndex = sceneIndex;
}

bool AudioManager::isMusicPlaying()
{
    sf::Music& currentMusic = *m_sceneMusic[currentSceneIndex];
    return currentMusic.getStatus() == sf::SoundSource::Playing;
}

void AudioManager::stopActiveMusic()
{
    if (m_sceneMusic[currentSceneIndex] == nullptr)
    {
        return;
    }

    sf::Music& currentMusic = *m_sceneMusic[currentSceneIndex];
    currentMusic.stop();
}