#include "sprite_object.h"

#include "e_memory.h"

#include "e_texture.h"

#include "e_vertex.h"

void SpriteObjectCreateQuad(SpriteObject *so)
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

    GraphicsObjectSetVertex(&so->go.graphObj, verts, 4, tIndx, 6);
    
    FreeMemory(verts);
    FreeMemory(tIndx);
}

int SpriteObjectInit(SpriteObject *so, DrawParam *dParam){

    GameObject2DInit((GameObject2D *)so, TIGOR_GAME_OBJECT_TYPE_2D);

    SpriteObjectCreateQuad(so);
    
    if(dParam->diffuse != NULL){        
        /// Textures init ---------------------------------------------------------------
        
        so->go.image = AllocateMemory(1, sizeof(GameObjectImage));

        TextureLoadImage(so, dParam->diffuse);
    }  

    so->go.graphObj.num_shapes = 1;
    
    return 1;
}

void SpriteObjectSetOffsetRect(SpriteObject *so, float x, float y, float width, float height)
{
    Vertex2D *verts = so->go.graphObj.shapes[0].vParam.vertices;

    float temp_x = x / so->go.image->imgWidth;
    float temp_y = y / so->go.image->imgHeight;

    width /= so->go.image->imgWidth;
    height /= so->go.image->imgHeight;
    
    printf ("Change Image dimensions %dx%d\n", so->go.image->imgWidth, so->go.image->imgHeight);

    verts[0].texCoord.x = temp_x;
    verts[0].texCoord.y = temp_y;

    verts[1].texCoord.x = temp_x + width;
    verts[1].texCoord.y = temp_y;

    verts[2].texCoord.x = temp_x + width;
    verts[2].texCoord.y = temp_y + height;

    verts[3].texCoord.x = temp_x;
    verts[3].texCoord.y = temp_y + height;
    
    glBindBuffer(GL_ARRAY_BUFFER, so->go.graphObj.shapes[0].vParam.buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * so->go.graphObj.shapes[0].vParam.num_verts, so->go.graphObj.shapes[0].vParam.vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

}
