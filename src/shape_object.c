#include "shape_object.h"

#include "e_camera.h"
#include "e_memory.h"

#include "e_math.h"

#include "e_texture.h"

#include "e_vertex.h"

extern TEngine engine;

void ShapeObjectCreateQuad(ShapeObject *so)
{
    Vertex2D *verts = AllocateMemory(4, sizeof(Vertex2D));

    verts[0].position.x = -1.0f;
    verts[0].position.y = -1.0f;
    verts[0].color = vec3_f(1, 1, 1);
    verts[0].texCoord.x = 0;
    verts[0].texCoord.y = 0;

    verts[1].position.x =  1.0f;
    verts[1].position.y = -1.0f;
    verts[1].color = vec3_f(1, 1, 1);
    verts[1].texCoord.x = 1.0f;
    verts[1].texCoord.y = 0;

    verts[2].position.x =  1.0f;
    verts[2].position.y =  1.0f;
    verts[2].color = vec3_f(1, 1, 1);
    verts[2].texCoord.x = 1.0f;
    verts[2].texCoord.y = 1.0f;

    verts[3].position.x = -1.0f;
    verts[3].position.y =  1.0f;
    verts[3].color = vec3_f(1, 1, 1);
    verts[3].texCoord.x = 0;
    verts[3].texCoord.y = 1.0f;


    uint32_t *tIndx = AllocateMemory(6, sizeof(uint32_t));

    uint32_t indx[] = {
      0, 1, 2, 2, 3, 0
    };

    memcpy(tIndx, indx, 6 * sizeof(uint32_t));

    GraphicsObjectSetVertex(&so->go.graphObj, verts, 4, tIndx, 6, TIGOR_VERTEX_TYPE_2D_OBJECT);
    
    FreeMemory(verts);
    FreeMemory(tIndx);
}

int ShapeObjectInit(ShapeObject *so, DrawParam *dParam, ShapeType type, void *param)
{
    char *name = "Shape_Object";

    memcpy(so->go.name, name, strlen(name));

    GameObject2DInit((GameObject2D *)so, TIGOR_GAME_OBJECT_TYPE_2D);

    switch(type)
    {
        case TIGOR_SHAPE_OBJECT_LINE:
            //ShapeObjectCreateLine(so, param);
            break;
        case TIGOR_SHAPE_OBJECT_QUAD:
            ShapeObjectCreateQuad(so);
            break;
        case TIGOR_SHAPE_OBJECT_CIRCLE:
            //ShapeObjectCreateCircle(so, param);
            break;
        case TIGOR_SHAPE_OBJECT_SHAPE:
            //ShapeObjectCreateShape(so, param);
            break;
        default:
            printf("Error Shape Type!\n");
            break;
    }

    so->type = type;
    
    if(dParam->diffuse != NULL){        
        /// Textures init ---------------------------------------------------------------
        
        so->go.image = AllocateMemory(1, sizeof(GameObjectImage));

        TextureLoadImage((GameObject *)so, dParam->diffuse);
        
    }  

    so->go.graphObj.num_shapes = 1;

    return 1;
}
