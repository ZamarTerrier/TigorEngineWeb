#include "graphicsObject.h"

#include "e_memory.h"

#include "e_vertex.h"

extern TEngine engine;

void GraphicsObjectInit(GraphicsObject* graphObj, uint32_t type)
{

    if(graphObj->num_shapes == 0)
        memset(&graphObj->shapes, 0, sizeof(Shape) * MAX_SHAPES);

    /*switch(type)
    {
        case TIGOR_VERTEX_TYPE_2D_OBJECT:
            graphObj->shapes[0].bindingDescription = &Bind2DDescription;
            graphObj->shapes[0].attr = planeAttributeDescription;
            graphObj->shapes[0].countAttr = 3;
            graphObj->shapes[0].countBind = 1;
            graphObj->shapes[0].type = type;
            break;
        case TIGOR_VERTEX_TYPE_3D_OBJECT:
            graphObj->shapes[0].bindingDescription = &Bind3DDescription;
            graphObj->shapes[0].attr = cubeAttributeDescription;
            graphObj->shapes[0].countAttr = 4;
            graphObj->shapes[0].countBind = 1;
            graphObj->shapes[0].type = type;
            break;
        case TIGOR_VERTEX_TYPE_3D_INSTANCE:
            graphObj->shapes[0].bindingDescription = AllocateMemory(2, sizeof(EIVertexInputBindingDescription));
            graphObj->shapes[0].bindingDescription[0] = Bind3DDescription;
            graphObj->shapes[0].bindingDescription[1] = Bind3DInstanceDescription;
            graphObj->shapes[0].attr = instanceAttributeDescription;
            graphObj->shapes[0].countAttr = 7;
            graphObj->shapes[0].countBind = 2;
            graphObj->shapes[0].type = type;
            break;
        case TIGOR_VERTEX_TYPE_TREE_INSTANCE:
            graphObj->shapes[0].bindingDescription = AllocateMemory(2, sizeof(EIVertexInputBindingDescription));
            graphObj->shapes[0].bindingDescription[0] = BindTree3DDescription;
            graphObj->shapes[0].bindingDescription[1] = Bind3DInstanceDescription;
            graphObj->shapes[0].attr = treeInstanceAttributeDescription;
            graphObj->shapes[0].countAttr = 7;
            graphObj->shapes[0].countBind = 2;
            graphObj->shapes[0].type = type;
            break;
        case TIGOR_VERTEX_TYPE_MODEL_OBJECT:
            graphObj->shapes[0].bindingDescription = &BindModel3DDescription;
            graphObj->shapes[0].attr = modelAttributeDescription;
            graphObj->shapes[0].countAttr = 7;
            graphObj->shapes[0].countBind = 1;
            graphObj->shapes[0].type = type;
            break;
        case TIGOR_VERTEX_TYPE_2D_PARTICLE:
            graphObj->shapes[0].bindingDescription = &BindParticle2DDescription;
            graphObj->shapes[0].attr = particle2DAttributeDescription;
            graphObj->shapes[0].countAttr = 3;
            graphObj->shapes[0].countBind = 1;
            graphObj->shapes[0].type = type;
            break;
        case TIGOR_VERTEX_TYPE_3D_PARTICLE:
            graphObj->shapes[0].bindingDescription = &BindParticle3DDescription;
            graphObj->shapes[0].attr = particle3DAttributeDescription;
            graphObj->shapes[0].countAttr = 3;
            graphObj->shapes[0].countBind = 1;
            graphObj->shapes[0].type = type;
            break;
        default:
            graphObj->shapes[0].type = 0;
            break;
    }*/
}

void GraphicsObjectSetVertex(GraphicsObject* graphObj, void *vertices, int vertCount, uint32_t *indices, int indxCount){

    uint32_t num = graphObj->num_shapes;

    int res = 0;

    if(vertices != NULL)
    {
        graphObj->shapes[num].vParam.vertices = AllocateMemoryP(vertCount, sizeof(Vertex2D), graphObj);
        memcpy(graphObj->shapes[num].vParam.vertices, vertices, sizeof(Vertex2D) * vertCount);
        graphObj->shapes[num].vParam.num_verts = vertCount;
    }

    if(indices != NULL)
    {
        graphObj->shapes[num].iParam.indices = AllocateMemoryP(indxCount, sizeof(uint32_t), graphObj);
        memcpy(graphObj->shapes[num].iParam.indices, indices, sizeof(uint32_t) * indxCount);
        graphObj->shapes[num].iParam.indexesSize = indxCount;
    }

    
    glGenVertexArraysOES(1, &graphObj->shapes[0].VAO);
    glGenBuffers(1, &graphObj->shapes[0].vParam.buffer);
    glGenBuffers(1, &graphObj->shapes[0].iParam.buffer);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArrayOES(graphObj->shapes[0].VAO);

    glBindBuffer(GL_ARRAY_BUFFER, graphObj->shapes[0].vParam.buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * graphObj->shapes[0].vParam.num_verts, graphObj->shapes[0].vParam.vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, graphObj->shapes[0].iParam.buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * graphObj->shapes[0].iParam.indexesSize, graphObj->shapes[0].iParam.indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
    
    glGenVertexArraysOES(0, NULL); 

    graphObj->num_shapes ++;
}

void GraphicsObjectSetShadersPath(GraphicsObject *graphObj, const char *vert, const char *frag)
{

    int len = strlen(vert);
    memset(graphObj->aShader.vertShader, 0, 256);
    memcpy(graphObj->aShader.vertShader, vert, len);


    len = strlen(frag);
    memset(graphObj->aShader.fragShader, 0, 256);
    memcpy(graphObj->aShader.fragShader, frag, len);
}
