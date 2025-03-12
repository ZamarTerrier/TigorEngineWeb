#include "engine.h"
#include "e_memory.h"
#include "e_camera.h"

#include "string.h"

#include "gameObject2D.h"

#include "e_window.h"

#include "e_texture_variables.h"

#include "GUIManager.h"
#include "window_manager.h"

TEngine engine;

extern void _wManagerPoolEventWeb(wManagerWindow *window, SDL_Event event);

SomeUpdateFunc Updater = NULL;

void EngineKeyCallback(wManagerWindow *window,  int key, int scancode, int action, int mods)
{
    //EntryWidgetKeyCallback(window, key, scancode, action, mods);

    for(int i=0; i < engine.func.keyCallbackSize;i++)
        engine.func.keyCallbacks[i](window, key, scancode, action, mods);
}

void EngineFingerCallback(wManagerWindow *window,  int fingerId, int action){

    for(int i=0; i < engine.func.fingerKeyCallbackSize;i++)
        engine.func.fingerKeyCallbacks[i](window, fingerId, action);
}

void EngineMouseKeyCallback(wManagerWindow *window,  int button, int action, int mods){

    for(int i=0; i < engine.func.mouseKeyCallbackSize;i++)
        engine.func.mouseKeyCallbacks[i](window, button, action, mods);
}

void main_loop() { 
    
    TWindow *window = (TWindow *)engine.window;
  
    SDL_GL_MakeCurrent(window->instance, engine.gl_context);

        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            _wManagerPoolEventWeb(window->e_window, e);
        }

        if(Updater != NULL)
            Updater(1.0);

            
        if(GUIManagerIsInit())
            GUIManagerUpdate();
            
        for( int i=0;i < engine.gameObjects.size;i++){
            if(!(engine.gameObjects.objects[i]->flags & TIGOR_GAME_OBJECT_FLAG_INIT))
                GameObjectInit(engine.gameObjects.objects[i]);
        }

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for( int i=0;i < engine.gameObjects.size;i++){
            GameObjectDraw(engine.gameObjects.objects[i]);
        }
        
        if(GUIManagerIsInit())
            GUIManagerDraw();
        
        engine.gameObjects.size = 0;

        SDL_GL_SwapWindow(window->instance);
                
        if(GUIManagerIsInit())
            GUIManagerClear();
 }

void EngineInit(){        
    engine.window = AllocateMemory(1, sizeof(TWindow));
    
    TWindow *window = (TWindow *)engine.window;

    wManagerInit();
    
    window->instance = SDL_CreateWindow(engine.app_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            engine.width, engine.height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
            
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetSwapInterval(1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    engine.gl_context = SDL_GL_CreateContext(window->instance);
    
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
           
    if(!GUIManagerIsInit()){        
        GUIManagerInit(true);
    }
    //memcpy(images[engine.DataR.e_var_num_images].path, text, strlen(text));
    engine.DataR.e_var_num_images ++;
}

void TEngineDraw(GameObject *go){

    if(engine.gameObjects.size > engine.gameObjects.curr_size - 1)
    {
        uint32_t new_size = engine.gameObjects.curr_size * 2;
        struct GameObject *point = AllocateMemoryP(new_size, sizeof(struct GameObject *), &engine);
        memcpy(point, engine.gameObjects.objects, sizeof(struct GameObject *) * engine.gameObjects.size);

        FreeMemory(engine.gameObjects.objects);

        engine.gameObjects.objects = (GameObject **)point;
        engine.gameObjects.curr_size = new_size;
    }

    for( int i=0;i < engine.gameObjects.size;i++){
        if(engine.gameObjects.objects[i] == go)
            return;
    }

    engine.gameObjects.objects[engine.gameObjects.size] = go;
    engine.gameObjects.size ++;
}

void TEngineSetFont(char *font_path){
    
    engine.DataR.font_path = font_path;

    GUIManagerInit(false);
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

void TEngineSetKeyCallback(SomeKeyCallbackFunc callback){
    engine.func.keyCallbackSize ++;

    engine.func.keyCallbacks = (e_keyCallback *)realloc(engine.func.keyCallbacks, engine.func.keyCallbackSize * sizeof(e_keyCallback));
    engine.func.keyCallbacks[engine.func.keyCallbackSize - 1] = (e_keyCallback)callback;
}

void TEngineSetMouseKeyCallback(SomeMouseKeyCallbackFunc callback){
    engine.func.mouseKeyCallbackSize ++;

    engine.func.mouseKeyCallbacks = (e_mouseKeyCallback *)realloc(engine.func.mouseKeyCallbacks, engine.func.mouseKeyCallbackSize * sizeof(e_mouseKeyCallback));
    engine.func.mouseKeyCallbacks[engine.func.mouseKeyCallbackSize - 1] = (e_mouseKeyCallback)callback;
}

void TEngineSetFingerCallback(SomeFingerKeyCallbackFunc callback){
    engine.func.fingerKeyCallbackSize ++;

    engine.func.fingerKeyCallbacks = (e_fingerKeyCallback *)realloc(engine.func.fingerKeyCallbacks, engine.func.fingerKeyCallbackSize * sizeof(e_fingerKeyCallback));
    engine.func.fingerKeyCallbacks[engine.func.fingerKeyCallbackSize - 1] = (e_fingerKeyCallback)callback;
}

void TEngineSetCursorPosCallback(void * callback){
    TWindow *window = (TWindow *)engine.window;

    wManagerSetCursorPosCallback(window->e_window, callback);
}

void TEngineSetUpdater(SomeUpdateFunc update){
    Updater = update;
}

void TEngineGetWindowSize(int *width, int *height){

    *width = engine.width;
    *height = engine.height;
}

extern int _wManagerGetMousePressState(wManagerWindow *window);
extern WebFinger *_wManagerGetFingerPressState(wManagerWindow *window);

int TEngineGetMousePressState(){
    TWindow *window = (TWindow *)engine.window;

    return _wManagerGetMousePressState(window->e_window);
}

void *TEngineGetFingerPressState(){
    TWindow *window = (TWindow *)engine.window;

    return _wManagerGetFingerPressState(window->e_window);
}

void TEngineRender(){
    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(main_loop, 0, true);
    #else
        while(true) main_loop();
    #endif
}
