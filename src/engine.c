#include "engine.h"
#include "e_memory.h"
#include "e_camera.h"

#include "string.h"

#include "gameObject2D.h"

TEngine engine;

void main_loop() { 
  
    SDL_GL_MakeCurrent(engine.window, engine.gl_context);

        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            int i;
            if(e.type == SDL_QUIT) i = 0;
        }

          
        for( int i=0;i < engine.gameObjects.size;i++){
            if(!(engine.gameObjects.objects[i]->flags & TIGOR_GAME_OBJECT_FLAG_INIT))
                GameObjectInit(engine.gameObjects.objects[i]);
        }


        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for( int i=0;i < engine.gameObjects.size;i++){
            GameObjectDraw(engine.gameObjects.objects[i]);
        }

        SDL_GL_SwapWindow(engine.window);
 }

void EngineInit(){        
    engine.window = SDL_CreateWindow(engine.app_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            engine.width, engine.height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
            
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetSwapInterval(1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    engine.gl_context = SDL_GL_CreateContext(engine.window);
    //SDL_CreateRenderer(engine.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    
}

void TEngineInitSystem(int width, int height, const char* name){
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

    //engine.DataR.e_var_images = AllocateMemoryP(MAX_IMAGES, sizeof(engine_buffered_image), &engine);
    engine.DataR.e_var_num_images = 0;

    engine.DataR.e_var_fonts = AllocateMemoryP(MAX_FONTS, sizeof(FontCache), &engine);
    engine.DataR.e_var_num_fonts = 0;
    
    EngineInit();
        
    //memcpy(images[engine.DataR.e_var_num_images].path, text, strlen(text));
    engine.DataR.e_var_num_images ++;
    
    memset(&engine.renders, 0, sizeof(EngineRenderItems));
}

void TEngineDraw(GameObject *go){

    for( int i=0;i < engine.gameObjects.size;i++){
        if(engine.gameObjects.objects[i] == go)
            return;
    }

    engine.gameObjects.objects[engine.gameObjects.size] = go;
    engine.gameObjects.size ++;
}

void TEngineRender(){
    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(main_loop, 0, true);
    #else
        while(true) main_loop();
    #endif
}
