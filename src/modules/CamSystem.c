#include "CamSystem.h"
#include <stdio.h>
#include "raymath.h"

static Vector3 camAnchor = {0};
static float smoothSpeed = 10.f;

void Cam_Update(Camera3D *cam, Body *body, Sol_Input *input, float dt)
{
    float distance = 10.0f;
    float shoulderSide = 1.0f;
    float shoulderHeight = 2.0f;
    
    // 1. Interpolate ONLY the base position (the "Lazy Root")
    // This smooths out player movement/jitter, but knows nothing about rotation
    float alpha = 1.0f - expf(-smoothSpeed * dt);
    
    camAnchor = Vector3Add(camAnchor, 
                    Vector3Scale(Vector3Subtract(body->position, camAnchor), alpha));

    // 2. Calculate the Snappy Offset (calculated every frame from fresh input)
    Vector3 flatFwd = Vector3Normalize((Vector3){ input->lookDir.x, 0, input->lookDir.z });
    Vector3 right = Vector3CrossProduct(flatFwd, (Vector3){0, 1, 0});
    
    // The "Shoulder Anchor" relative to our lazy root
    Vector3 anchor = camAnchor;
    anchor.y += shoulderHeight;
    anchor = Vector3Add(anchor, Vector3Scale(right, shoulderSide));

    // 3. Apply to Camera
    // Target is snappy relative to the anchor; Position is snappy relative to the target
    cam->target = anchor;
    cam->position = Vector3Subtract(cam->target, Vector3Scale(input->lookDir, distance));
}