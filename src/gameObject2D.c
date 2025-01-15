#include "gameObject2D.h"

#include <math.h>

#include "e_memory.h"
#include "e_camera.h"

#include "e_math.h"

#include "e_vertex.h"

const char *vertexShaderSource = 
      "attribute vec2 position;   \n"
      "attribute vec3 color;   \n"
      "attribute vec2 textCoord;\n"
      "uniform mat4 u_model; \n"
      "uniform mat4 u_view; \n"
      "uniform mat4 u_proj; \n"
      "varying vec2 vTextureCoords;\n"
      "varying vec3 vFragColor;\n"
      "void main()                 \n"
      "{                           \n"
      "   gl_Position = u_proj * u_view * u_model * vec4(position, 0.0, 1.0); \n"
      "   vTextureCoords = textCoord;\n"
      "   vFragColor = color;\n"
      "}                           \n";
const char *fragmentShaderSource = 
#ifdef EMSCRIPTEN
      "precision highp float;                   \n"
#endif
      "uniform sampler2D u_texture;\n"
      "varying vec2 vTextureCoords;\n"
      "varying vec3 vFragColor;\n"
      "void main()                                \n"
      "{                                          \n"
      "  vec4 color =  texture2D(u_texture, vTextureCoords); \n"
      "  //color.rgb *=  vFragColor;                \n"
      "  gl_FragColor = color;  \n"
      "}                                          \n";

extern TEngine engine;

void GameObject2DDefaultUpdate(GameObject2D* go) {    
        Camera2D* cam = (Camera2D*) engine.cam2D;    
        

        // draw our first triangle
        glUseProgram(go->self.shaderProgram);

        mat4 model = m4_transform2D(go->transform.position, go->transform.scale, go->transform.rotation);
        unsigned int tMat = glGetUniformLocation(go->self.shaderProgram, "u_model");
        glUniformMatrix4fv(tMat, 1, GL_FALSE, (const float*) &model);
        
        mat4 view = m4_transform2D(v2_muls(cam->position, -1), cam->scale, cam->angle);
        tMat = glGetUniformLocation(go->self.shaderProgram, "u_view");
        glUniformMatrix4fv(tMat, 1, GL_FALSE, (const float*) &view);
        
        mat4 proj = m4_ortho(0, engine.width, engine.height, 0.0, -1.0, 1.0f);
        tMat = glGetUniformLocation(go->self.shaderProgram, "u_proj");
        glUniformMatrix4fv(tMat, 1, GL_FALSE, (const float*) &proj);
                
        if(go->image != NULL){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, go->image->texture_id);
        }
        
}

void GameObject2DDefaultDraw(GameObject2D* go){
        
    for(int i=0; i < go->graphObj.num_shapes;i++){   
        glBindVertexArrayOES(go->graphObj.shapes[i].VAO);        
        glDrawElements(GL_TRIANGLES, go->graphObj.shapes[i].iParam.indexesSize, GL_UNSIGNED_INT, 0);

    }
}

void GameObject2DSetShader(GameObject2D *go, char *vert_source, char *frag_source){
    
     // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vert_source, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");// << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &frag_source, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");// << infoLog << std::endl;
    }
    // link shaders
    go->self.shaderProgram = glCreateProgram();
    glAttachShader(go->self.shaderProgram, vertexShader);
    glAttachShader(go->self.shaderProgram, fragmentShader);
    glLinkProgram(go->self.shaderProgram);
    // check for linking errors
    glGetProgramiv(go->self.shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(go->self.shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");// << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    go->self.flags |= TIGOR_GAME_OBJECT_FLAG_SHADED;
}

void GameObject2DInitDefaultShader(GameObject2D *go){    
    
    if(go->self.flags & TIGOR_GAME_OBJECT_FLAG_SHADED)
        return;
    
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");// << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");// << infoLog << std::endl;
    }
    // link shaders
    go->self.shaderProgram = glCreateProgram();
    glAttachShader(go->self.shaderProgram, vertexShader);
    glAttachShader(go->self.shaderProgram, fragmentShader);
    glLinkProgram(go->self.shaderProgram);
    // check for linking errors
    glGetProgramiv(go->self.shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(go->self.shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");// << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
        
    go->self.flags |= TIGOR_GAME_OBJECT_FLAG_SHADED;
}

void GameObject2DInitDraw(GameObject2D *go)
{
    if(!(go->self.flags & TIGOR_GAME_OBJECT_FLAG_SHADED))
        return;

    go->self.flags |= TIGOR_GAME_OBJECT_FLAG_INIT;
}

void GameObject2DInitDefault(GameObject2D *go){

    GameObjectShaderInit(go);

    GameObject2DInitDraw(go);
}

void GameObject2DClean(GameObject2D* go){
    //GraphicsObjectClean(&go->graphObj);
}

void GameObject2DRecreate(GameObject2D* go){

    //GameObject2DClean(go);

    Transform2DReposition((struct GameObject2D_T *)go);
    Transform2DRescale((struct GameObject2D_T *)go);

}

void GameObject2DDestroy(GameObject2D* go){

    //GraphicsObjectDestroy(&go->graphObj);

    if(go->image != NULL)
    {
        FreeMemory(go->image->path);

        if(go->image->size > 0)
            FreeMemory(go->image->buffer);

        FreeMemory(go->image);
    }

    for(int i=0; i < go->graphObj.num_shapes; i++)
    {
        if(go->graphObj.shapes[i].vParam.num_verts)
            FreeMemory(go->graphObj.shapes[i].vParam.vertices);

        if(go->graphObj.shapes[i].iParam.indexesSize)
            FreeMemory(go->graphObj.shapes[i].iParam.indices);
    }

    FreeMemory(go->self.vert);
    FreeMemory(go->self.frag);
    
    go->self.flags &= ~(TIGOR_GAME_OBJECT_FLAG_INIT);
}

void GameObject2DInit(GameObject2D* go, GameObjectType type)
{
    GameObjectSetInitFunc((GameObject *)go, (void *)GameObject2DInitDefault);
    GameObjectSetUpdateFunc((GameObject *)go, (void *)GameObject2DDefaultUpdate);
    GameObjectSetDrawFunc((GameObject *)go, (void *)GameObject2DDefaultDraw);
    GameObjectSetCleanFunc((GameObject *)go, (void *)GameObject2DClean);
    GameObjectSetRecreateFunc((GameObject *)go, (void *)GameObject2DRecreate);
    GameObjectSetDestroyFunc((GameObject *)go, (void *)GameObject2DDestroy);
    
    GameObjectSetShaderInitFunc((GameObject *)go, (void *)GameObject2DInitDefaultShader);

    go->self.obj_type = type;

    Transform2DInit(&go->transform);

    switch(type){
        case TIGOR_GAME_OBJECT_TYPE_2D:
            GraphicsObjectInit(&go->graphObj, TIGOR_VERTEX_TYPE_2D_OBJECT);
            break;
        case TIGOR_GAME_OBJECT_TYPE_PARTICLE_2D:
            GraphicsObjectInit(&go->graphObj, TIGOR_VERTEX_TYPE_2D_PARTICLE);
            break;
    }

}
