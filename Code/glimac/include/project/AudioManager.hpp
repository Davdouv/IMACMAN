#pragma once
#include <SDL/SDL_mixer.h>
#include <vector>
#include <iostream>

class AudioManager {

public:
    // Constructor
    AudioManager();
    // Destructor
    ~AudioManager();

    // Initializer, called in constructor
    void initAudio();
    // Close audio, called in destructor
    void closeAudio();

    // Music Functions
    Mix_Music* createMusic(const char* filename);
    void addMusic(Mix_Music* music);
    //void playMusic(Mix_Music* music);
    void playMusic(unsigned int i);
    void freeMusic(Mix_Music* music);

    // Sound functions
    Mix_Chunk* createSound(const char* filename);
    void addSound(Mix_Chunk* sound);
    //void playSound(unsigned int channel, Mix_Chunk* sound);
    void playSound(unsigned int i, unsigned int time);
    void freeSound(Mix_Chunk* sound);

    // Fill m_sound vector
    void fillSounds();

private:
    std::vector<Mix_Music*> m_music;
    std::vector<Mix_Chunk*> m_sound;
};
