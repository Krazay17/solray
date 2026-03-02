#include "CamSystem.h"
#include "raymath.h"

void Update_Cam(Camera3D *cam, Body *body, CamControl *ctrl)
{
    // 1. Get Mouse Delta (How much did the mouse move this frame?)
    Vector2 mouseDelta = GetMouseDelta();

    // 2. Update Euler Angles
    ctrl->yaw += mouseDelta.x * ctrl->sens;
    ctrl->pitch -= mouseDelta.y * ctrl->sens;

    // 3. Clamp Pitch (Prevent the camera from flipping upside down)
    if (ctrl->pitch > 89.0f)
        ctrl->pitch = 89.0f;
    if (ctrl->pitch < -89.0f)
        ctrl->pitch = -89.0f;

    // 4. Calculate Direction Vector from Angles
    Vector3 direction;
    direction.x = cosf(DEG2RAD * ctrl->yaw) * cosf(DEG2RAD * ctrl->pitch);
    direction.y = sinf(DEG2RAD * ctrl->pitch);
    direction.z = sinf(DEG2RAD * ctrl->yaw) * cosf(DEG2RAD * ctrl->pitch);

    // 5. Positioning (Third Person Offset)
    float distance = 10.0f;
    cam->target = body->position;
    // Position is Target MINUS the direction vector scaled by distance
    cam->position = Vector3Subtract(cam->target, Vector3Scale(direction, distance));
}