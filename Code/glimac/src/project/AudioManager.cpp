#include "project/AudioManager.hpp"

// --- CONSTRUCTOR | DESTRUCTOR --- //

// CONSTRUCTOR
AudioManager::AudioManager()
{
    initAudio();
}
// DESTRUCTOR
AudioManager::~AudioManager()
{
    for(unsigned int i = 0; i < m_music.size(); ++i)
        freeMusic(m_music[i]);
    for (unsigned int i = 0; i < m_sound.size(); ++i)
        freeSound(m_sound[i]);
    closeAudio();
}
// INITIALIZE AUDIO
void AudioManager::initAudio()
{
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
    {
        printf("%s", Mix_GetError());
    }
    else
    {
        Mix_VolumeMusic(MIX_MAX_VOLUME / 8);
        Mix_AllocateChannels(10);
    }
}
// CLOSE AUDIO
void AudioManager::closeAudio()
{
    Mix_CloseAudio();
}

// --- MUSIC --- //

// CREATE MUSIC
Mix_Music * AudioManager::createMusic(const char* filename)
{
    Mix_Music *music;
    music = Mix_LoadMUS(filename);
    return music;
}

// ADD MUSIC ON THE MUSIC LIST
void AudioManager::addMusic(Mix_Music* music)
{
    m_music.push_back(music);
}
// PLAY A MUSIC
// void AudioManager::playMusic(Mix_Music* music)
// {
//     Mix_PlayMusic(music, -1);
// }
void AudioManager::playMusic(unsigned int i)
{
    if (m_music.size() >= i+1)
        Mix_PlayMusic(m_music[i], -1);//playMusic(m_music[i]);
    else
        std::cout << "There's not enough music" << std::endl;
}
// FREE MUSIC
void AudioManager::freeMusic(Mix_Music* music)
{
    Mix_FreeMusic(music);
}

// --- SOUNDS --- //

// CREATE SOUND
Mix_Chunk* AudioManager::createSound(const char* filename)
{
    Mix_Chunk * sound;
    sound = Mix_LoadWAV(filename);
    Mix_VolumeChunk(sound, MIX_MAX_VOLUME/10);
    return sound;
}

// ADD SOUND
void AudioManager::addSound(Mix_Chunk* sound)
{
    m_sound.push_back(sound);
}

// PLAY SOUND
// void AudioManager::playSound(unsigned int channel, Mix_Chunk* sound)
// {
//     while(Mix_Playing(channel))
//         ++channel;
//     Mix_Volume(channel, MIX_MAX_VOLUME);
//     Mix_PlayChannel(channel, sound, 0);
// }
void AudioManager::playSound(unsigned int i, unsigned int time)
{
    if (m_sound.size() >= i+1)
    {
        unsigned int channel = i;
        // Find a channel that is not played
        while(Mix_Playing(channel))
            ++channel;
        Mix_Volume(channel, MIX_MAX_VOLUME);
        Mix_PlayChannel(channel, m_sound[i], time-1);
    }
    else
        std::cout << "There's not enough sound" << std::endl;
}

// FREE SOUND
void AudioManager::freeSound(Mix_Chunk* sound)
{
    Mix_FreeChunk(sound);
}

// Here we create all the sounds
void AudioManager::fillSounds()
{
    addSound(createSound("../Code/assets/audio/select.wav"));
    addSound(createSound("../Code/assets/audio/eat4.wav"));
    addSound(createSound("../Code/assets/audio/hit2.wav"));
    addSound(createSound("../Code/assets/audio/eatGhost.wav"));
    addSound(createSound("../Code/assets/audio/super.wav"));
}