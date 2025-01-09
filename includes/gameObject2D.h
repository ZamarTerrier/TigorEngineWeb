#ifndef GAMEOBJECT2D_H
#define GAMEOBJECT2D_H

#include "graphicsObject.h"
#include "e_transform.h"


#include "gameObject.h"


#ifdef __cplusplus
extern "C"
{
#endif

typedef struct{
    GameObject self;
    GraphicsObject graphObj;
    Transform2D transform;
    GameObjectImage *image;
    uint32_t num_images;
    char name[256];
} GameObject2D;

void GameObject2DTransformBufferUpdate(GameObject2D *go, void *data);
void GameObject2DImageBuffer(GameObject2D *go, void *data);

void GameObject2DDefaultUpdate(GameObject2D* go);
void GameObject2DDefaultDraw(GameObject2D* go);

void GameObject2DSetShader(GameObject2D *go, char *vert_path, char *frag_path);
void GameObject2DSetShaderSimple(GameObject2D *go, char *vert_path, char *frag_path);

void GameObject2DInitDefault(GameObject2D *go);

void GameObject2DInitDraw(GameObject2D *go);
void GameObject2DInit(GameObject2D* go, GameObjectType type);
void GameObject2DClean(GameObject2D* go);
void GameObject2DRecreate(GameObject2D* go);
void GameObject2DDestroy(GameObject2D* go);

vec2 GameObject2DGetSize(GameObject2D* go);

#ifdef __cplusplus
}
#endif

#endif // GAMEOBJECT2D_H
