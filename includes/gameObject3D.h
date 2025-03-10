#ifndef GAMEOBJECT3D_H
#define GAMEOBJECT3D_H

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
    Transform3D transform;
    GameObjectImage *image;
    uint32_t num_images;
    char name[256];
} GameObject3D;

void GameObject3DTransformBufferUpdate(GameObject3D *go, void *data);
void GameObject3DImageBuffer(GameObject3D *go, void *data);

void GameObject3DDefaultUpdate(GameObject3D* go);
void GameObject3DDefaultDraw(GameObject3D* go);

void GameObject3DSetShader(GameObject3D *go, const char *vert_path, const char *frag_path);

void GameObject3DInitDefault(GameObject3D *go);

void GameObject3DInitDraw(GameObject3D *go);
void GameObject3DInit(GameObject3D* go, GameObjectType type);
void GameObject3DClean(GameObject3D* go);
void GameObject3DRecreate(GameObject3D* go);
void GameObject3DDestroy(GameObject3D* go);

vec2 GameObject3DGetSize(GameObject3D* go);

#ifdef __cplusplus
}
#endif

#endif // GAMEOBJECT3D_H
