#include "Loader.h"
#include "ResourceLoader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define RLIGHTS_IMPLEMENTATION
#include "models/rlights.h"

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

    res->shaders.light = LoadShaderFromRes("ID_SND_LIGHTSHADERV", "ID_SND_LIGHTSHADERF");

    res->models.defMat = LoadMaterialDefault();
    res->models.cylinderMesh = GenMeshCylinder(0.5f, 1.0f, 8);
    res->models.cylinderModel = LoadModelFromMesh(res->models.cylinderMesh);
    res->models.wizardModel = LoadModelFromRes(
        "ID_SND_WIZARD",
        ".glb",
        &res->models.wizardAnims,
        &res->models.wizardAnimCount);
    for (int i = 0; i < res->models.wizardModel.materialCount; i++)
    {
        res->models.wizardModel.materials[i].shader = res->shaders.light;
    }
}

Shader LoadShaderFromRes(const char *vsId, const char *fsId)
{
    RawResource vsRes = LoadResourceData(vsId);
    RawResource fsRes = LoadResourceData(fsId);

    // We must ensure the strings are null-terminated for the GLSL compiler
    char *vsSource = (char *)malloc(vsRes.size + 1);
    char *fsSource = (char *)malloc(fsRes.size + 1);

    memcpy(vsSource, vsRes.data, vsRes.size);
    memcpy(fsSource, fsRes.data, fsRes.size);

    vsSource[vsRes.size] = '\0';
    fsSource[fsRes.size] = '\0';

    // LoadShaderCode takes raw strings instead of file paths
    Shader shdr = LoadShaderFromMemory(vsSource, fsSource);

    free(vsSource);
    free(fsSource);

    return shdr;
}

Model LoadModelFromRes(const char *id, const char *ext, ModelAnimation **anims, int *animCount)
{
    RawResource res = LoadResourceData(id);
    if (res.data && res.size > 0)
    {
        // 1. Create a unique temporary filename
        // In a real app, use GetTempPath() on Windows, but this is simple:
        char tempPath[256];
        sprintf(tempPath, "temp_model_%s%s", id, ext);

        // 2. Write the raw resource bytes to this temp file
        FILE *file = fopen(tempPath, "wb");
        if (file)
        {
            fwrite(res.data, 1, res.size, file);
            fclose(file);

            // 3. Tell raylib to load from the file we just created
            Model model = LoadModel(tempPath);
            *anims = LoadModelAnimations(tempPath, animCount);

            // 4. Clean up the physical file immediately
            // The model data is now safely in GPU/System RAM
            remove(tempPath);

            return model;
        }
    }
    return (Model){0};
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

        if (res->models.wizardAnims != NULL)
        {
            UnloadModelAnimations(res->models.wizardAnims, res->models.wizardAnimCount);
        }

        UnloadModel(res->models.cylinderModel);
        UnloadModel(res->models.wizardModel);

        free(res);
        res = NULL;
    }
}