
#include "engine.h"

#include "e_camera.h"

#include "shape_object.h"
#include "sprite_object.h"

Camera2D cam2D;

ShapeObject shape;

SpriteObject sprite;

int main(int argc, char** argv)
{   

    TEngineInitSystem(1280, 1024, "Test");

    Camera2DInit(&cam2D);

    DrawParam dParam;
    memset(&dParam, 0, sizeof(dParam));

    dParam.diffuse = "assets/texture.png";
    
    ShapeObjectInit(&shape, &dParam, TIGOR_SHAPE_OBJECT_QUAD, NULL);
    Transform2DSetScale(&shape, 100, 100);
    Transform2DSetPosition(&shape, 200, 200);
    
    dParam.diffuse = "assets/hero.png";

    SpriteObjectInit(&sprite, &dParam);
    Transform2DSetScale(&sprite, 50, 50);
    Transform2DSetPosition(&sprite, 100, 100);

    SpriteObjectSetOffsetRect(&sprite, 0, 0, 100, 120);

    TEngineDraw(&shape);
    TEngineDraw(&sprite);
    
    TEngineRender();

    return 0;
}