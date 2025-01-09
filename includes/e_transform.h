#ifndef E_TRANSFORM_H
#define E_TRANSFORM_H

#include "engine_includes.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct{
    vec2 position;
    vec2 positionOrig;
    float rotation;
    vec2 scale;
    vec2 scaleOrig;
    struct{
        vec2 offset;
        vec2 scale;
        float rotation;
    }img;
    mat4 model;
} Transform2D;

typedef struct{
    vec3 position;
    vec3 rotation;
    vec3 scale;
    struct{
        vec2 offset;
        vec2 scale;
    }img;
    mat4 model;
} Transform3D;

struct GameObject2D_T;
struct GameObject3D_T;

void Transform2DInit(Transform2D* transform);
void Transform3DInit(Transform3D* transform);

void Transform2DSetPosition(struct GameObject2D_T* go, float x, float y);
vec2 Transform2DGetPosition(struct GameObject2D_T* go);
void Transform2DReposition(struct GameObject2D_T* go);
void Transform2DSetImageOffset(struct GameObject2D_T* go, float x, float y);
vec2 Transform2DGetImageOffset(struct GameObject2D_T* go);
void Transform2DSetScale(struct GameObject2D_T* go, float x, float y);
vec2 Transform2DGetScale(struct GameObject2D_T* go);
void Transform2DRescale(struct GameObject2D_T* go);
void Transform2DSetRotation(struct GameObject2D_T* go, float angle);
float Transform2DGetRotation(struct GameObject2D_T* go);

void Transform3DSetPositionT(Transform3D* transform, float x, float y, float z);
void Transform3DSetPosition(struct GameObject3D_T* go, float x, float y, float z);
vec3 Transform3DGetPosition(struct GameObject3D_T* go);

void Transform3DSetScaleT(Transform3D* transform, float x, float y, float z);
void Transform3DSetScale(struct GameObject3D_T* go, float x, float y, float z);
vec3 Transform3DGetScale(struct GameObject3D_T* go);

void Transform3DSetRotationT(Transform3D* transform, float x, float y, float z);
void Transform3DSetRotation(struct GameObject3D_T* go, float x, float y, float z);
vec3 Transform3DGetRotation(struct GameObject3D_T* go);

#ifdef __cplusplus
}
#endif

#endif // E_TRANSFORM_H
