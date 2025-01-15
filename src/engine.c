#include "engine.h"
#include "e_memory.h"
#include "e_camera.h"

#include "string.h"

#include "gameObject2D.h"

#include "e_window.h"

#include "e_texture_variables.h"

#include "window_manager.h"

TEngine engine;

extern void _wManagerPoolEventWeb(wManagerWindow *window, SDL_Event event);

SomeUpdateFunc Updater = NULL;

void EngineKeyCallback(wManagerWindow *window,  uint32_t key, uint32_t scancode, uint32_t action, uint32_t mods)
{
    //EntryWidgetKeyCallback(window, key, scancode, action, mods);

    for(int i=0; i < engine.func.keyCallbackSize;i++)
        engine.func.keyCallbacks[i](window, key, scancode, action, mods);
}

void main_loop() { 
    
    TWindow *window = engine.window;
  
    SDL_GL_MakeCurrent(window->instance, engine.gl_context);

        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            _wManagerPoolEventWeb(window->e_window, e);
        }

        if(Updater != NULL)
            Updater(1.0);
          
        for( int i=0;i < engine.gameObjects.size;i++){
            if(!(engine.gameObjects.objects[i]->flags & TIGOR_GAME_OBJECT_FLAG_INIT))
                GameObjectInit(engine.gameObjects.objects[i]);
        }

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for( int i=0;i < engine.gameObjects.size;i++){
            GameObjectDraw(engine.gameObjects.objects[i]);
        }
        
        engine.gameObjects.size = 0;

        SDL_GL_SwapWindow(window->instance);
 }

void EngineInit(){        
    engine.window = AllocateMemory(1, sizeof(TWindow));
    
    TWindow *window = engine.window;

    wManagerInit();

    window->instance = SDL_CreateWindow(engine.app_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            engine.width, engine.height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
            
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetSwapInterval(1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    engine.gl_context = SDL_GL_CreateContext(window->instance);
    //SDL_CreateRenderer(engine.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    
}

void TEngineInitSystem(int width, int height, const char* name){
    TWindow *window = (TWindow *)engine.window;

    memset(&engine, 0, sizeof(TEngine));

    strcpy(engine.app_name, name);

    engine.present = true;

    engine.width = width;
    engine.height = height;

    engine.diffSize = vec2_f( 1.0f, 1.0f );

    engine.viewSize.x = engine.width;
    engine.viewSize.y = engine.height;

    //rootDirPath = e_GetCurrectFilePath();

    engine.cache.alloc_buffers_memory_head = calloc(1, sizeof(ChildStack));
    engine.cache.alloc_descriptor_head = calloc(1, sizeof(ChildStack));
    engine.cache.alloc_pipeline_head = calloc(1, sizeof(ChildStack));

    engine.e_var_current_entry = NULL;

    engine.MAX_FRAMES_IN_FLIGHT = 3;

    engine.DataR.e_var_images = AllocateMemoryP(MAX_IMAGES, sizeof(engine_buffered_image), &engine);
    engine.DataR.e_var_fonts = AllocateMemoryP(MAX_FONTS, sizeof(FontCache), &engine);
    engine.gameObjects.objects = AllocateMemoryP(START_DRAW_OBJECTS, sizeof(struct GameObject *), &engine);
    engine.gameObjects.curr_size = START_DRAW_OBJECTS;
    
    EngineInit();
    
    wManagerSetKeyCallback(window->e_window, EngineKeyCallback);
        
    //memcpy(images[engine.DataR.e_var_num_images].path, text, strlen(text));
    engine.DataR.e_var_num_images ++;
    
    memset(&engine.renders, 0, sizeof(EngineRenderItems));
}

void TEngineDraw(GameObject *go){

    if(engine.gameObjects.size > engine.gameObjects.curr_size - 1)
    {
        uint32_t new_size = engine.gameObjects.curr_size * 2;
        struct GameObject *point = AllocateMemoryP(new_size, sizeof(struct GameObject *), &engine);
        memcpy(point, engine.gameObjects.objects, sizeof(struct GameObject *) * engine.gameObjects.size);

        FreeMemory(engine.gameObjects.objects);

        engine.gameObjects.objects = point;
        engine.gameObjects.curr_size = new_size;
    }

    for( int i=0;i < engine.gameObjects.size;i++){
        if(engine.gameObjects.objects[i] == go)
            return;
    }

    engine.gameObjects.objects[engine.gameObjects.size] = go;
    engine.gameObjects.size ++;
}

int TEngineGetMousePress(int Key){
    TWindow *window = (TWindow *)engine.window;

    int state = 0;

#ifndef __ANDROID__
    state = wManagerGetMouseButton(window->e_window, Key);
#endif

    return state;
}

void TEngineGetCursorPos(double *xpos, double *ypos){
    TWindow *window = (TWindow *)engine.window;

#ifndef __ANDROID__
    wManagerGetCursorPos(window->e_window, xpos, ypos);
#endif
}

void TEngineSetCursorPos(float xpos, float ypos){
    TWindow *window = (TWindow *)engine.window;

#ifndef __ANDROID__
    wManagerSetCursorPos(window->e_window, xpos, ypos);
#endif
}

int TEngineGetKeyPress(int Key){
    TWindow *window = (TWindow *)engine.window;

    int state = 0;

#ifndef __ANDROID__
    state =  wManagerGetKey(window->e_window, Key);
#endif

    return state;
}

void TEngineSetKeyCallback(void *callback){
    engine.func.keyCallbackSize ++;

    engine.func.keyCallbacks = (e_keyCallback *)realloc(engine.func.keyCallbacks, engine.func.keyCallbackSize * sizeof(e_keyCallback));
    engine.func.keyCallbacks[engine.func.keyCallbackSize - 1] = (e_keyCallback)callback;
}

void TEngineSetMouseKeyCallback(void *callback){
    TWindow *window = (TWindow *)engine.window;

    wManagerSetMouseButtonCallback(window->e_window, callback);
}

void TEngineSetCursorPosCallback(void * callback){
    TWindow *window = (TWindow *)engine.window;

    wManagerSetCursorPosCallback(window->e_window, callback);
}

void TEngineSetUpdater(SomeUpdateFunc *update){
    Updater = update;
}

void TEngineRender(){
    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(main_loop, 0, true);
    #else
        while(true) main_loop();
    #endif
}
