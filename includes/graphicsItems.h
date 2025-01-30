#ifndef GRAPHICSITEMS_H
#define GRAPHICSITEMS_H

#include "engine_includes.h"

#define MAX_SHAPES 32

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
    TIGOR_SHADER_OBJECT_NONE = 0x1,
    TIGOR_SHADER_OBJECT_READED = 0x2
} ShaderObjectFlags;

typedef struct{
    char* code;
    size_t size;
    ShaderObjectFlags flags;
} ShaderObject;

typedef struct{
    uint32_t buffer;
    uint32_t attribute;
    void* vertices;
    uint32_t num_verts;
} vertexParam;

typedef struct{
    uint32_t buffer;
    uint32_t indexesSize;
    uint32_t* indices;
} indexParam;

typedef struct{
    char vertShader[256];
    char fragShader[256];
} aboutShader;

typedef struct{
    uint32_t VAO;
    indexParam iParam;
    vertexParam vParam;
    uint32_t countBind;
    uint32_t countAttr;

    uint32_t type;

    bool init;
} Shape;

enum EngineDescriptorType{
    TIGOR_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    TIGOR_DESCRIPTOR_TYPE_IMAGE_SAMPLER,
    TIGOR_DESCRIPTOR_TYPE_IMAGE_ARRAY
};

#ifdef __cplusplus
}
#endif

#endif // GRAPHICSITEMS_H
