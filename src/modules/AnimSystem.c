#include "AnimSystem.h"
#include "core/GameWorld.h"

void Anim_Update(Sol_Anim *anims, Entities *entities, float dt)
{
    for (int i = 0; i < entities->count; i++)
    {
        if (!entities->active[i])
            continue;

        Sol_Anim *anim = &anims[i];

        // Check if the current animation actually has bone data
        if (anim->anims[anim->currentAnim].boneCount == 0)
            continue;
        if (anim->model == NULL)
            continue;

        // Update animation logic
        anim->animTime += dt * 30; // 30 FPS playback

        // Wrap time using the frameCount from the animation
        if (anim->animTime >= anim->anims[anim->currentAnim].keyframeCount)
        {
            anim->animTime = 0;
        }

        UpdateModelAnimation(*anim->model, anim->anims[anim->currentAnim], (int)anim->animTime);
    }
}