/*
 * Handles sound output using the SDl library.
 */

#ifndef __SoundSystem_h_
#define __SoundSystem_h_

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_audio.h>

class SoundSystem
{
protected:
    Mix_Chunk *wallHit;
    Mix_Chunk *raquetHit;
    Mix_Chunk *targetHit;
    Mix_Chunk *heliHit;

public:
    SoundSystem(void);
    void playWallHit(void);
    void playRaquetHit(void);
    void playTargetHit(void);
    void playHeliHit(void);
};

#endif //#ifndef __SoundSystem_h_
