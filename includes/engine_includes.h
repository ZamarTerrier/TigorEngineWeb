#ifndef ENGINEINCLUDES_H
#define ENGINEINCLUDES_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __EMSCRIPTEN__

#include <emscripten.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#include <SDL_opengles2.h>

#else

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

#endif

#include "e_math.h"

#include "key_defines.h"

#define MAX_FONTS 256
#define MAX_IMAGES 256

#define START_DRAW_OBJECTS 256

typedef void (*e_charCallback)(void *, uint32_t);
typedef void (*e_keyCallback)(void *, int , int , int , int );


typedef enum{
    TIGOR_VERTEX_TYPE_NONE,
    TIGOR_VERTEX_TYPE_2D_OBJECT,
    TIGOR_VERTEX_TYPE_3D_OBJECT,
    TIGOR_VERTEX_TYPE_3D_INSTANCE,
    TIGOR_VERTEX_TYPE_TREE_INSTANCE,
    TIGOR_VERTEX_TYPE_MODEL_OBJECT,
    TIGOR_VERTEX_TYPE_2D_PARTICLE,
    TIGOR_VERTEX_TYPE_3D_PARTICLE,
    TIGOR_VERTEX_TYPE_TERRAIN,
} VertexType;

typedef struct ChildStack{
    struct ChildStack* next;
    struct ChildStack* before;
    void *node;
} ChildStack;

typedef struct{
    char *path;
    char *buffer;
    int size;
    int imgWidth;
    int imgHeight;
    uint32_t img_type;
    uint32_t texture_id;
    uint32_t flags;
} GameObjectImage;

typedef struct{
    struct RenderTexture *objects[START_DRAW_OBJECTS];
    uint32_t size;
} EngineRenderItems;

typedef struct{
    struct GameObject **objects;
    uint32_t curr_size;
    uint32_t size;    
} EngineGameObjects;

typedef struct FontCache{
    char path[256];
    void *cdata;
    void *info;
    void *texture;
} FontCache;

typedef struct{
    char *diffuse;
    char *specular;
    char *normal;
    char font[256];
    char topology;
    char flags;
} DrawParam;

typedef struct{
    char app_name[256];
    int width;
    int height;
    void* cam2D;
    void* cam3D;
    
    vec2 viewSize;
    vec2 diffSize;

    struct TWindow_T *window;
    struct TDevice_T *device;
    struct TSwapChain_T *swapchain;

    EngineRenderItems renders;

    EngineGameObjects gameObjects;    

    struct{
        void (*DrawFunc)(void);
        void (*RecreateFunc)(void);

        e_charCallback *charCallbacks;
        int charCallbackSize;

        e_keyCallback *keyCallbacks;
        int keyCallbackSize;
    } func;
    
    struct{
        ChildStack *alloc_buffers_memory_head;
        ChildStack *alloc_descriptor_head;
        ChildStack *alloc_pipeline_head;
    } cache;

    
    void *e_var_current_entry;

    void *main_render;    
    void *current_render;

    size_t currentFrame;
    uint32_t imageIndex;
    
    uint32_t extensionCount;
    uint32_t imagesCount;
    uint32_t wManagerExtensionCount;

    bool framebufferResized;
    bool framebufferwasResized;

    void* debugMessenger;

    bool present;

    uint32_t MAX_FRAMES_IN_FLIGHT;

    void *gl_context;

    struct DataR{        
        char *font_path;
        void *e_var_images;
        int e_var_num_images;

        FontCache *e_var_fonts;
        uint32_t e_var_num_fonts;

        int define_font_loaded;
    } DataR;

} TEngine;

#endif //ENGINEINCLUDES_H