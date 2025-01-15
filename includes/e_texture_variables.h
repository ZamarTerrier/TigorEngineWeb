#ifndef E_TEXTURE_VARIABLES_H
#define E_TEXTURE_VARIABLES_H
#include "engine_includes.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum{
    TIGOR_TEXTURE2D_FLAG_GENERATED = 0x1,
    TIGOR_TEXTURE2D_FLAG_VIEW = 0x2,
    TIGOR_TEXTURE2D_IS_FONT = 0x4,
} EngineTexture2DFlag;

typedef struct{
    char *path;
    char *buffer;
    char *data;
    uint32_t buff_size;
    uint32_t texWidth;
    uint32_t texHeight;
    uint32_t texChannels;
    uint32_t mip_levels;
} ImageFileData;

typedef struct{
    uint32_t textureType;
    uint32_t imageLayout;
    ImageFileData image_data;
    EngineTexture2DFlag flags;
} Texture2D;

typedef struct{
    char path[1024];
    Texture2D texture;
    uint32_t buffer;
} engine_buffered_image;

#ifdef __cplusplus
}
#endif

#endif // E_TEXTURE_VARIABLES_H
