#include "SoundSystem.h"
#include "iostream"
#include "Ball.h"

SoundSystem::SoundSystem()
{
    // Initialize SDL functionality using recommended settings
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

    // Load the music and begin playing
    music = Mix_LoadMUS("media/sound/HeartBeatMusic.wav");
    Mix_PlayMusic(music, -1);

    // allocate 16 mixing channels
    Mix_AllocateChannels(16);

    // Load the sound effects
    wallHit = Mix_LoadWAV("media/sound/wallHit.wav");
    raquetHit = Mix_LoadWAV("media/sound/raquetHit.wav");
    targetHit = Mix_LoadWAV("media/sound/targetHit.wav");
	p2Hit = Mix_LoadWAV("media/sound/p2grunt.wav");
}

/* Play the sounds */

void SoundSystem::playMusic(void)
{
    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(music, -1);
    } else if (Mix_PausedMusic() == 1) {
        Mix_ResumeMusic();
    } else {
        Mix_PauseMusic();
    }
}

void SoundSystem::resetFlags(void) {
    flags = NOSOUND;
}

void SoundSystem::playWallHit(void)
{
    if (flags != BALLWALL) {
        Mix_PlayChannel(-1, wallHit, 0);
    }
    flags = BALLWALL; 
}

void SoundSystem::playRaquetHit(void)
{
    if (flags != SERVERBALLPADDLE) {
        static int count = 0;
        if (-1 == Mix_PlayChannel(-1, raquetHit, 0)) {
            std::cout << count++ << " play raquet hit failed!!!!!! :(" << std::endl;
        }
    }
    flags = SERVERBALLPADDLE;
}

void SoundSystem::playTargetHit(void)
{
    if (flags != BALLTARGET) {
        Mix_PlayChannel(-1, targetHit, 0);
    }
    flags = BALLTARGET;
}

void SoundSystem::playP2Hit(void)
{
    if (flags != CLIENTBALLPADDLE) {
        Mix_PlayChannel(-1, p2Hit, 0);
    }
    flags = CLIENTBALLPADDLE;
}
