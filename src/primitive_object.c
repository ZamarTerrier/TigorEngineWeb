#include "primitive_object.h"


#include "e_memory.h"
#include "e_camera.h"
#include "e_texture.h"
#include "e_vertex.h"

#include "e_tools.h"

extern TEngine engine;

const Vertex3D cubeVert[] = {
    //Top
    {{-1.0f, 1.0f, -1.0}, {0.0, 0.0, 1.0}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
    {{1.0f, 1.0f, -1.0}, {0.0, 0.0, 1.0}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0}, {0.0, 0.0, 1.0}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
    {{-1.0f, 1.0f, 1.0}, {0.0, 0.0, 1.0}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
    //Front
    {{-1.0f, 1.0f, 1.0f}, {1.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
    {{1.0f, -1.0f, 1.0f}, {1.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
    {{-1.0f, -1.0f, 1.0f}, {1.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
    //Left
    {{-1.0f, 1.0f, 1.0}, {0.0, 1.0, 0.0}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
    {{-1.0f, -1.0f, 1.0}, {0.0, 1.0, 0.0}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
    {{-1.0f, -1.0f, -1.0}, {0.0, 1.0, 0.0}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
    {{-1.0f, 1.0f, -1.0}, {0.0, 1.0, 0.0}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
    //Down
    {{-1.0f, -1.0f, -1.0}, {0.0, 0.0, -1.0}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
    {{-1.0f, -1.0f, 1.0}, {0.0, 0.0, -1.0}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
    {{1.0f, -1.0f, 1.0}, {0.0, 0.0, -1.0}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
    {{1.0f, -1.0f, -1.0}, {0.0, 0.0, -1.0}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
    //Back
    {{-1.0f, 1.0f, -1.0f}, {-1.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
    {{-1.0f, -1.0f, -1.0f}, {-1.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
    {{1.0f, -1.0f, -1.0f}, {-1.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
    {{1.0f, 1.0f, -1.0f}, {-1.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
    //Right
    {{1.0f, 1.0f, 1.0}, {0.0, -1.0, 0.0}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
    {{1.0f, 1.0f, -1.0}, {0.0, -1.0, 0.0}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
    {{1.0f, -1.0f, -1.0}, {0.0, -1.0, 0.0}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
    {{1.0f, -1.0f, 1.0}, {0.0, -1.0, 0.0}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
};

const uint32_t cubeIndx[] = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    8, 9, 10, 10, 11, 8,//
    12, 13, 14, 14, 15, 12,
    16, 17, 18, 18, 19, 16,
    20, 21, 22, 22, 23, 20//
};

void PrimitiveObjectDestroy(PrimitiveObject *po)
{
    GameObject3DDestroy((GameObject3D *)po);

    FreeMemory(po->params);
    po->params = NULL;
}

int PrimitiveObjectInit(PrimitiveObject *po, DrawParam *dParam, char type, void *params){

    GameObject3DInit((GameObject3D *)po, TIGOR_GAME_OBJECT_TYPE_3D);

    GameObjectSetDestroyFunc((GameObject *)po, (void *)PrimitiveObjectDestroy);

    po->type = type;

    PlaneParam *pParam = (PlaneParam *)params;
    SphereParam *sParam = (SphereParam *)params;
    CubeSphereParam *csParam = (CubeSphereParam *)params;
    ConeParam *cParam = (ConeParam *)params;

    int builded = false;

    vertexParam vParam;
    indexParam iParam;

    po->params = NULL;
    switch(type)
    {
        case TIGOR_PRIMITIVE3D_LINE :
            //GraphicsObjectSetVertex(&po->go.graphObj, (void *)lineVert, 2, sizeof(Vertex3D), NULL, 0, sizeof(uint32_t));
            break;
        case TIGOR_PRIMITIVE3D_TRIANGLE :
            //GraphicsObjectSetVertex(&po->go.graphObj, (void *)triVert, 3, sizeof(Vertex3D), (uint32_t *)triIndx, 3, sizeof(uint32_t));
            break;
        case TIGOR_PRIMITIVE3D_QUAD :
            //GraphicsObjectSetVertex(&po->go.graphObj, (void *)quadVert, 4, sizeof(Vertex3D), (uint32_t *)quadIndx, 6, sizeof(uint32_t));
            break;
        case TIGOR_PRIMITIVE3D_PLANE :
            InitPlane3D(&vParam, &iParam, pParam->sectorCount, pParam->stackCount);
            po->params = AllocateMemory(1, sizeof(PlaneParam));
            memcpy(po->params, params, sizeof(PlaneParam));
            builded = true;
            break;
        case TIGOR_PRIMITIVE3D_CUBE :
            GraphicsObjectSetVertex(&po->go.graphObj, (void *)cubeVert, 24, (uint32_t *)cubeIndx, 36, TIGOR_VERTEX_TYPE_3D_OBJECT);
            break;
        case TIGOR_PRIMITIVE3D_CUBESPHERE :
            Cubesphere(&vParam, &iParam, csParam->radius, csParam->verperrow);
            po->params = AllocateMemory(1, sizeof(CubeSphereParam));
            memcpy(po->params, params, sizeof(CubeSphereParam));
            builded = true;
            break;
        case TIGOR_PRIMITIVE3D_ICOSPHERE :
            IcoSphereGenerator(&vParam, &iParam, sParam->radius);
            po->params = AllocateMemory(1, sizeof(SphereParam));
            memcpy(po->params, params, sizeof(SphereParam));
            break;
        case TIGOR_PRIMITIVE3D_SPHERE :
            SphereGenerator3D(&vParam, &iParam, sParam->radius, sParam->sectorCount, sParam->stackCount);
            po->params = AllocateMemory(1, sizeof(SphereParam));
            memcpy(po->params, params, sizeof(SphereParam));
            builded = true;
            break;
        case TIGOR_PRIMITIVE3D_CONE :
            ConeGenerator(&vParam, &iParam, cParam->height, cParam->sectorCount, cParam->stackCount);
            po->params = AllocateMemory(1, sizeof(ConeParam));
            memcpy(po->params, params, sizeof(ConeParam));
            builded = true;
            break;
        case TIGOR_PRIMITIVE3D_SKYBOX:
            SphereGenerator3D(&vParam, &iParam, sParam->radius, sParam->sectorCount, sParam->stackCount);
            po->params = AllocateMemory(1, sizeof(SphereParam));
            memcpy(po->params, params, sizeof(SphereParam));
            builded = true;
            break;
    }

    if(builded)
    {
        GraphicsObjectSetVertex(&po->go.graphObj, vParam.vertices, vParam.num_verts, iParam.indices, iParam.indexesSize, TIGOR_VERTEX_TYPE_3D_OBJECT);
        FreeMemory(vParam.vertices);
        FreeMemory(iParam.indices);
    }

    po->type = type;
    
    if(dParam->diffuse != NULL){        
        /// Textures init ---------------------------------------------------------------
        
        po->go.image = AllocateMemory(1, sizeof(GameObjectImage));

        TextureLoadImage((GameObject *)po, dParam->diffuse);
        
    }  
    
    if(type == TIGOR_PRIMITIVE3D_SKYBOX)
        Transform3DSetScale((struct GameObject3D_T *)po, -500, -500, -500);
    
    po->go.graphObj.num_shapes = 1;

    return 1;
}
