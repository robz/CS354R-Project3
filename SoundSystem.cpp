#include "SoundSystem.h"

SoundSystem::SoundSystem()
{
    // Initialize SDL functionality using recommended settings
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

    // Load the music and begin playing
    music = Mix_LoadMUS("media/sound/HeartBeatMusic.wav");
    Mix_PlayMusic(music, -1);

    // Load the sound effects
    wallHit = Mix_LoadWAV("media/sound/wallHit.wav");
    raquetHit = Mix_LoadWAV("media/sound/raquetHit.wav");
    targetHit = Mix_LoadWAV("media/sound/targetHit.wav");
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

void SoundSystem::playWallHit(void)
{
    Mix_PlayChannel(-1, wallHit, 0);
}

void SoundSystem::playRaquetHit(void)
{
    Mix_PlayChannel(-1, raquetHit, 0);
}

void SoundSystem::playTargetHit(void)
{
    Mix_PlayChannel(-1, targetHit, 0);
}
