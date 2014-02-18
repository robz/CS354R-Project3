#include "SoundSystem.h"

SoundSystem::SoundSystem()
{
    // Initialize SDL functionality using recommended settings
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

    // Load the WAV sound files located in the media/sound directory
    wallHit = Mix_LoadWAV("media/sound/wallHit.wav");
    raquetHit = Mix_LoadWAV("media/sound/raquetHit.wav");
    targetHit = Mix_LoadWAV("media/sound/targetHit.wav");
    heliHit = Mix_LoadWAV("media/sound/heliHit.wav");
}

/* Play the sounds */

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

void SoundSystem::playHeliHit(void)
{
    Mix_PlayChannel(-1, heliHit, 0);
}
