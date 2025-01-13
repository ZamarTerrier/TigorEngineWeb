
#include "engine.h"

#include "e_camera.h"

#include "shape_object.h"
#include "sprite_object.h"

#include "lib_yandex.h"

#include "mxjson.h"
#include "mxstr.h"
#include "mxutil.h"

Camera2D cam2D;

ShapeObject shape;

SpriteObject sprite;

int offsetX = 0, offsetY = 0;

float ticker = 1.0f, playerSpeed = 0.3f;

int idle = true;

bool ready = false;


void GetCatalog(const int success, const char *jsons){
    if(!success)
        printf("Something wrong!\n");
    else{        
        mxjson_parser_t p;
        mxjson_token_t *t;
        mxbuf_t buffer;
        mxstr_t str;

        bool valid;
        int i;

        size_t json_size = strlen(jsons);
        str = mxstr(jsons, json_size);
        // Инициализация парсера
        mxjson_init(&p, 0, NULL, mxjson_resize);

        // Парсинг JSON строки
        valid = mxjson_parse(&p, str);
        if (valid) {
            // Цикл по всем токенам в JSON
            for (i = 1; i <= p.idx; i++) {
                t = &p.tokens[i];
                // Вывод имени и значения токена
                printf("Имя: %s, Значение: %s\n", mxjson_token_name(&p, i, &buffer, &valid), mxjson_token_string(&p, i, &buffer, &valid));
            }
        }

        // Очистка памяти и завершение работы парсера
        mxjson_free(&p);
        
    }

    free(jsons);
}

void ShowFullAdv(const int callbackType, const char *data){

    switch(callbackType){
        case 0:
            printf("Adv Open!\n");
            break;
        case 1:
            printf("Adv Close!\n");
            break;
        case 2:
            printf("Adv Error! %s\n", data);
            break;
        case 3:
            printf("Adv OnOffline!\n");
            break;
    }

    free(data);
}

void UpdateFunc(float deltaTime){

    vec2 pos = Transform2DGetPosition(&sprite);
    
    idle = true;
    
    if(TEngineGetKeyPress(TIGOR_KEY_W)){
        pos.y -= playerSpeed;
        offsetY = 6;
        idle = false;
    }else if(TEngineGetKeyPress(TIGOR_KEY_S)){
        pos.y += playerSpeed;
        offsetY = 4;
        idle = false;
    }
    
    if(TEngineGetKeyPress(TIGOR_KEY_D)){
        pos.x += playerSpeed;
        offsetY = 7;
        idle = false;
    }else if(TEngineGetKeyPress(TIGOR_KEY_A)){
        pos.x -= playerSpeed;
        offsetY = 5;
        idle = false;
    }

    if(TEngineGetKeyPress(TIGOR_KEY_F))
        JS_GetCatalog(GetCatalog);
        
    if(TEngineGetKeyPress(TIGOR_KEY_R))
        JS_GameplayAPI_Start();
    
    if(TEngineGetKeyPress(TIGOR_KEY_Q))
        JS_GameplayAPI_Stop();
        
    if(TEngineGetKeyPress(TIGOR_KEY_P))
        JS_ShowFullscreenAdv(ShowFullAdv);
    if(TEngineGetKeyPress(TIGOR_KEY_T))
        JS_ShowRewardedVideo(ShowFullAdv);
    
    Transform2DSetPosition(&sprite, pos.x, pos.y);

    if(idle)
        offsetY = 0;

    if(ticker < 0.0)
    {
        offsetX ++;
        ticker = 1.0f;

        if(!idle){
            if(offsetX > 9)
                offsetX = 0;
        }
        else{
            if(offsetX > 2)
                offsetX = 0;
        }
    }

    SpriteObjectSetOffsetRect(&sprite, offsetX * 102, offsetY * 130, 100, 130);

    TEngineDraw(&shape);
    TEngineDraw(&sprite);

    ticker -= 0.1f;
    
}

int main(int argc, char** argv)
{   

    TEngineInitSystem(1280, 720, "Test");

    TEngineSetUpdater((SomeUpdateFunc)UpdateFunc);

    Camera2DInit(&cam2D);

    DrawParam dParam;
    memset(&dParam, 0, sizeof(dParam));

    dParam.diffuse = "assets/texture.png";
    
    ShapeObjectInit(&shape, &dParam, TIGOR_SHAPE_OBJECT_QUAD, NULL);
    Transform2DSetScale(&shape, 100, 100);
    Transform2DSetPosition(&shape, 200, 200);
    
    dParam.diffuse = "assets/hero.png";

    SpriteObjectInit(&sprite, &dParam);
    Transform2DSetScale(&sprite, 20, 20);
    Transform2DSetPosition(&sprite, 100, 100);

    SpriteObjectSetOffsetRect(&sprite, 0, 0, 100, 120);
    
    TEngineRender();
    

    return 0;
}