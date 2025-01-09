#include "e_camera.h"

#include "e_math.h"

extern TEngine engine;

void Camera2DInit(Camera2D *cam){

    memset(cam, 0, sizeof(Camera2D));

    cam->scale.x = 1;
    cam->scale.y = 1;

    if(engine.cam2D == NULL)
        Camera2DSetActive(cam);
}

void Camera3DInit(Camera3D *cam){

    memset(cam, 0, sizeof(Camera3D));

    cam->scale.x = 1;
    cam->scale.y = 1;
    cam->scale.z = 1;
    cam->view_distance = 1000;
    cam->view_angle = 75.0f;
    cam->view_near = 0.01f;

    cam->yaw = 90;
    cam->pitch = 0;
    cam->sensitivity = 2.0f;

    cam->moveSpeed = 1.0f;
    cam->cameraSpeed = 1.5f;

    cam->firstMouse = true;
    cam->walk = false;
    
    if(engine.cam3D == NULL)
        Camera3DSetActive(cam);
    
    vec3 next_rotation;

    next_rotation.x = cos(90 * (M_PI / 180)) * cos(0 * (M_PI / 180));
    next_rotation.y = -sin(0 * (M_PI / 180));
    next_rotation.z = sin(90 * (M_PI / 180)) * cos(0 * (M_PI / 180));
    
    next_rotation = v3_norm(next_rotation);

    Camera3DSetRotation(next_rotation.x, next_rotation.y, next_rotation.z);
}

void Camera3DSetViewDistance(Camera3D *cam, double distance)
{
    cam->view_distance = distance;
}

void Camera3DSetViewAngle(Camera3D *cam, double angle)
{
    cam->view_angle = angle;
}

void Camera3DSetViewNear(Camera3D *cam, double v_near)
{
    cam->view_near = v_near;
}

void Camera3DSetActive(Camera3D *cam){
    engine.cam3D = cam;
}

void Camera2DSetActive(Camera2D *cam){
    engine.cam2D = cam;
}

void Camera2DSetRotation(float angle){
    Camera2D* cam = (Camera2D*)engine.cam2D;

    cam->angle = angle;
}

void Camera3DSetRotation(float x, float y, float z){
    Camera3D* cam = (Camera3D*)engine.cam3D;

    cam->rotation.x = x;
    cam->rotation.y = y;
    cam->rotation.z = z;
}

float Camra2DGetRotation(){
    Camera2D* cam = (Camera2D*)engine.cam2D;

    return cam->angle;
}

vec3 Camera3DGetRotation(){
    Camera3D* cam = (Camera3D*)engine.cam3D;

    return cam->rotation;
}

void Camera2DSetPosition(float x, float y){
    Camera2D* cam = (Camera2D*)engine.cam2D;

    cam->position.x = x;
    cam->position.y = y;
}

void Camera3DSetPosition(float x, float y, float z){
    Camera3D* cam = (Camera3D*)engine.cam3D;

    cam->position.x = x;
    cam->position.y = y;
    cam->position.z = z;
}

vec2 Camera2DGetPosition(){
    Camera2D* cam = (Camera2D*)engine.cam2D;

    return cam->position;
}

vec3 Camera3DGetPosition(){
    Camera3D* cam = (Camera3D*)engine.cam3D;

    return cam->position;
}

void Camera2DSetScale(float x, float y){
    Camera2D* cam = (Camera2D*)engine.cam2D;

    cam->scale.x = x;
    cam->scale.y = y;
}

void Camera3DSetScale(float x, float y, float z){
    Camera3D* cam = (Camera3D*)engine.cam3D;

    cam->scale.x = x;
    cam->scale.y = y;
    cam->scale.z = z;
}

void Camera3DMovementUpdate(float deltaTime){
    
}

void Camera3DRotationUpdate(float deltaTime){
    
}

void Camera3DSetLockCursor(){
    
}

bool Camera3DGetLockCursor(){
    Camera3D* cam = (Camera3D*)engine.cam3D;

    return cam->lockMouse;
}