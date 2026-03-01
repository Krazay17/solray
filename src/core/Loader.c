#include "Loader.h"
#include "ResourceLoader.h"
#include <stdlib.h>

static ResourceManager *res = NULL;

ResourceManager *GetRM(void)
{
    return res;
}

void sol_init_loader(void)
{
    res = malloc(sizeof(ResourceManager));

    res->audio.pistolFire = LoadSoundFromRes("ID_SND_GUN", ".mp3");
    res->audio.hitSound = LoadSoundFromRes("ID_SND_HIT", ".wav");
    res->audio.digi = LoadSoundFromRes("ID_SND_DIGI", ".mp3");
    res->audio.beep1 = LoadSoundFromRes("ID_SND_BEEP1", ".wav");
    res->audio.beep2 = LoadSoundFromRes("ID_SND_BEEP2", ".wav");
    res->audio.menuMusic = LoadSoundFromRes("ID_SND_MUSIC", ".mp3");
    res->audio.woong1 = LoadSoundFromRes("ID_SND_WOONG", ".wav");

    res->draw.defMat = LoadMaterialDefault();
    Mesh cylMesh = res->draw.cylinderMesh = GenMeshCylinder(0.5f, 1.0f, 8);
    res->draw.cylinderModel = LoadModelFromMesh(cylMesh);
}

Sound LoadSoundFromRes(const char *id, const char *ext)
{
    RawResource res = LoadResourceData(id);
    if (res.data)
    {
        Wave wave = LoadWaveFromMemory(ext, res.data, res.size);
        Sound snd = LoadSoundFromWave(wave);
        UnloadWave(wave);
        return snd;
    }
    return (Sound){0};
}

void CloseLoader(void)
{
    if (res)
    {
        UnloadSound(res->audio.pistolFire);
        UnloadSound(res->audio.hitSound);
        UnloadSound(res->audio.digi);
        UnloadSound(res->audio.beep1);
        UnloadSound(res->audio.beep2);
        UnloadSound(res->audio.woong1);
        UnloadSound(res->audio.menuMusic);

        UnloadModel(res->draw.cylinderModel);
        UnloadMesh(res->draw.cylinderMesh);

        free(res);
        res = NULL;
    }
}