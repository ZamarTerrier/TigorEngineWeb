#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H

#include "graphicsItems.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct{
    aboutShader aShader;
    bool lock;
    Shape shapes[MAX_SHAPES];
    uint32_t num_shapes;
} GraphicsObject;

void GraphicsObjectInit(GraphicsObject* graphObj, uint32_t type);

void GraphicsObjectSetShaderWithUniform(GraphicsObject* graphObj, ShaderObject *shader, uint32_t pack_indx);
void GraphicsObjectSetShader(GraphicsObject* graphObj, ShaderObject *shader, uint32_t pack_ind, uint32_t shader_type);

void GraphicsObjectSetVertex(GraphicsObject* graphObj, void *vertices, int vertCount, uint32_t *indices, int indxCount);

void GraphicsObjectCreateDrawItems(GraphicsObject* graphObj);
void GraphicsObjectCleanPipelines(GraphicsObject *graphObj);
void GraphicsObjectClean(GraphicsObject* graphObj);
void GraphicsObjectDestroy(GraphicsObject* graphObj);

void GraphicsObjectSetShadersPath(GraphicsObject* graphOb, const char* vert, const char* frag);

#ifdef __cplusplus
}
#endif

#endif // GRAPHICSOBJECT_H
