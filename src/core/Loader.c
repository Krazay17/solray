#include "Loader.h"
#include <stdlib.h>
#include "hexed/test_sound.h"
#include "hexed/SpaceGun.h"
#include "hexed/HitSound.h"

static ResourceManager *res = NULL;

ResourceManager *GetRM(void)
{
    return res;
}

void sol_init_loader(void)
{
    res = malloc(sizeof(ResourceManager));

    Wave flashAwayWave = LoadWaveFromMemory(".mp3", Test1_mp3, Test1_mp3_len);
    Wave pistolFireWave = LoadWaveFromMemory(".mp3", SpaceGun_mp3, SpaceGun_mp3_len);
    Wave hitSoundWave = LoadWaveFromMemory(".wav", Hit_wav, Hit_wav_len);

    res->audio.flashAway = LoadSoundFromWave(flashAwayWave);
    res->audio.pistolFire = LoadSoundFromWave(pistolFireWave);
    res->audio.hitSound = LoadSoundFromWave(hitSoundWave);
}

void CloseLoader(void) {
    if (res) {
        UnloadSound(res->audio.flashAway);
        UnloadSound(res->audio.pistolFire);
        UnloadSound(res->audio.hitSound);
        free(res);
        res = NULL;
    }
}