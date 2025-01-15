
#include "engine.h"

#include "e_camera.h"
#include "e_memory.h"
#include "e_math.h"

#include "shape_object.h"
#include "sprite_object.h"

#include "lib_yandex.h"

#include "jansson.h"

#include <stdio.h>

int TILE_MAP_WIDTH = 25;
int TILE_MAP_HEIGHT = 19;

Camera2D cam2D;

ShapeObject shape;

SpriteObject sprite;

int offsetX = 0, offsetY = 0;

float ticker = 1.0f, playerSpeed = 0.5f;

int idle = true;

bool ready = false;

SpriteObject **tiles;

int **layers;
int num_layers = 0;


void GetCatalog(const int success, const char *jsons){
    if(!success)
        printf("Something wrong!\n");
    else{        
        
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

/* forward refs */
void print_json(json_t *root);
void print_json_aux(json_t *element, int indent);
void print_json_indent(int indent);
const char *json_plural(size_t count);
void print_json_object(json_t *element, int indent);
void print_json_array(json_t *element, int indent);
void print_json_string(json_t *element, int indent);
void print_json_integer(json_t *element, int indent);
void print_json_real(json_t *element, int indent);
void print_json_true(json_t *element, int indent);
void print_json_false(json_t *element, int indent);
void print_json_null(json_t *element, int indent);

void print_json(json_t *root) { print_json_aux(root, 0); }

void print_json_aux(json_t *element, int indent) {
    switch (json_typeof(element)) {
        case JSON_OBJECT:
            print_json_object(element, indent);
            break;
        case JSON_ARRAY:
            print_json_array(element, indent);
            break;
        case JSON_STRING:
            print_json_string(element, indent);
            break;
        case JSON_INTEGER:
            print_json_integer(element, indent);
            break;
        case JSON_REAL:
            print_json_real(element, indent);
            break;
        case JSON_TRUE:
            print_json_true(element, indent);
            break;
        case JSON_FALSE:
            print_json_false(element, indent);
            break;
        case JSON_NULL:
            print_json_null(element, indent);
            break;
        default:
            fprintf(stderr, "unrecognized JSON type %d\n", json_typeof(element));
    }
}

void print_json_indent(int indent) {
    int i;
    for (i = 0; i < indent; i++) {
        putchar(' ');
    }
}

const char *json_plural(size_t count) { return count == 1 ? "" : "s"; }

void print_json_object(json_t *element, int indent) {
    size_t size;
    const char *key;
    json_t *value;

    print_json_indent(indent);
    size = json_object_size(element);

    printf("JSON Object of %lld pair%s:\n", (long long)size, json_plural(size));
    json_object_foreach(element, key, value) {
        
        if(!strcmp(key, "layers")){
            print_json_indent(indent + 2);
            size_t size = json_array_size(value);

            layers = AllocateMemory(size, sizeof(double *));
            
            print_json_aux(value, indent + 2);
        }else if(!strcmp(key, "data")){
            print_json_indent(indent + 2);
            size_t size = json_array_size(value);

            layers[num_layers] = AllocateMemory(size, sizeof(double));
            for (int i = 0; i < size; i++) {
                layers[num_layers][i] = json_integer_value(json_array_get(value, i));

            }

            num_layers ++;
            
            printf("JSON Array \"%s\" Data of %lld element%s:\n", key, (long long)size, json_plural(size));
        }else if(!strcmp(key, "height")){
            TILE_MAP_HEIGHT = json_integer_value(value);
        }else if(!strcmp(key, "width")){
            TILE_MAP_WIDTH = json_integer_value(value);
        }else{
            print_json_indent(indent + 2);
            printf("JSON Key: \"%s\"\n", key);
            print_json_aux(value, indent + 2);
        }
    }
}

void print_json_array(json_t *element, int indent) {
    size_t i;
    size_t size = json_array_size(element);
    print_json_indent(indent);

    printf("JSON Array of %lld element%s:\n", (long long)size, json_plural(size));
    for (i = 0; i < size; i++) {
        print_json_aux(json_array_get(element, i), indent + 2);
    }
}

void print_json_string(json_t *element, int indent) {
    print_json_indent(indent);
    printf("JSON String: \"%s\"\n", json_string_value(element));
}

void print_json_integer(json_t *element, int indent) {
    print_json_indent(indent);
    printf("JSON Integer: \"%" JSON_INTEGER_FORMAT "\"\n", json_integer_value(element));
}

void print_json_real(json_t *element, int indent) {
    print_json_indent(indent);
    printf("JSON Real: %f\n", json_real_value(element));
}

void print_json_true(json_t *element, int indent) {
    (void)element;
    print_json_indent(indent);
    printf("JSON True\n");
}

void print_json_false(json_t *element, int indent) {
    (void)element;
    print_json_indent(indent);
    printf("JSON False\n");
}

void print_json_null(json_t *element, int indent) {
    (void)element;
    print_json_indent(indent);
    printf("JSON Null\n");
}


/*
 * Parse text into a JSON object. If text is valid JSON, returns a
 * json_t structure, otherwise prints and error and returns null.
 */
json_t *load_json(const char *text) {
    json_t *root;
    json_error_t error;

    root = json_loads(text, 0, &error);

    if (root) {
        return root;
    } else {
        fprintf(stderr, "json error on line %d: %s\n", error.line, error.text);
        return (json_t *)0;
    }
}

void LoadTileMap(const char *namefile){


    FILE* fd;
    int size;

    fd = fopen(namefile, "r");
    if (fd == NULL) {
        printf("File Not Found!\n");
        return;
    }    

    fseek(fd, 0L, SEEK_END);
    size = ftell(fd);

    char *buff = (char *)AllocateMemory(size, sizeof(char));

    fseek(fd, 0L, SEEK_SET);

    fread(buff, sizeof(char), size, fd);

    fflush(fd);

    fclose(fd);
            
    json_t *root = load_json(buff);

    if (root) {
        /* print and release the JSON structure */
        print_json(root);
        json_decref(root);
    }
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

    vec2 p_pos = Transform2DGetPosition(&sprite);
    vec2 c_pos = Camera2DGetPosition();

    
    if(TEngineGetKeyPress(TIGOR_KEY_G))
        printf("Player position x : %f | y : %f\n", p_pos.x, p_pos.y);

    if(p_pos.x - 400 > 0)
        c_pos.x = p_pos.x - 400;

    
    if(p_pos.y - 300 > 0)
        c_pos.y = p_pos.y - 300;

    Camera2DSetPosition(c_pos.x, c_pos.y);

    p_pos.x = floor(p_pos.x * 64) / 64 / 50;
    p_pos.y = floor(p_pos.y * 64) / 64 / 50;

    
    if(TEngineGetKeyPress(TIGOR_KEY_G))
        printf("Player tile position x : %f | y : %f\n", p_pos.x, p_pos.y);

    for(int i=0;i < num_layers;i++){
        for(int y = (int)p_pos.y < 6 ? 0 : (int)p_pos.y - 6; y < (int)p_pos.y + 12;y++)
        {
            if(y < 0 || y > TILE_MAP_HEIGHT - 1)
                continue;

            for(int x =(int) p_pos.x < 10 ? 0 : (int)p_pos.x - 10; x < (int)p_pos.x + 20;x++)
            {
                
                if(x < 0 || x > TILE_MAP_WIDTH - 1)
                    continue;
                    
                TEngineDraw(&tiles[i][(y * TILE_MAP_WIDTH) + x]);
            }
        }
    }

    //TEngineDraw(&shape);
    TEngineDraw(&sprite);
    

    ticker -= 0.1f;
    
}

int main(int argc, char** argv)
{   

    LoadTileMap("assets/Island.tmj");

    printf("Tilemap size w : %i, h : %i\n", TILE_MAP_WIDTH, TILE_MAP_HEIGHT);


    tiles = AllocateMemory(num_layers, sizeof(SpriteObject *));

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
    
    dParam.diffuse = "assets/tiles.png";

    int tile_size_x = 64, tile_size_y = 64;
    int tile_scale_x = 64, tile_scale_y = 64;

    int offX = 0, offY = 0;
    for(int i=0;i < num_layers;i++){
        tiles[i] = AllocateMemory(TILE_MAP_HEIGHT * TILE_MAP_WIDTH, sizeof(SpriteObject));
        for(int y = 0; y < TILE_MAP_HEIGHT;y++)
        {
            for(int x = 0; x < TILE_MAP_WIDTH;x++)
            {

                SpriteObjectInit(&tiles[i][(y * TILE_MAP_WIDTH) + x ], &dParam);
                Transform2DSetScale(&tiles[i][(y * TILE_MAP_WIDTH) + x ], tile_scale_x / 2, tile_scale_y / 2);
                Transform2DSetPosition(&tiles[i][(y * TILE_MAP_WIDTH) + x ], x * tile_scale_x + 32, y * tile_scale_y + 32);

                int s_val = layers[i][(y * TILE_MAP_WIDTH) + x ];

                if(s_val == 0){
                    offX = 15;
                    offY = 0;
                }else{
                    offX = 0;
                    offY = 0;                    

                    while(s_val > 16)
                    {                        
                        offY ++;
                        s_val -= 16;
                    }

                    s_val --;

                    offX = s_val;
                }

                SpriteObjectSetOffsetRect(&tiles[i][(y * TILE_MAP_WIDTH) + x ], offX * tile_size_x, offY * tile_size_y, tile_size_x, tile_size_y);
            }
        }
    }

    TEngineRender();
    

    return 0;
}