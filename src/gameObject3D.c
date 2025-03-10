#include "gameObject3D.h"

#include <math.h>

#include "e_memory.h"
#include "e_camera.h"

#include "e_math.h"

#include "e_vertex.h"

const char *vertexShaderSource = 
      "attribute vec3 position;   \n"
      "attribute vec3 normal;   \n"
      "attribute vec3 color;   \n"
      "attribute vec2 textCoord;\n"
      "uniform mat4 u_model; \n"
      "uniform mat4 u_view; \n"
      "uniform mat4 u_proj; \n"
      "varying vec2 vTextureCoords;\n"
      "varying vec3 vFragColor;\n"
      "void main()                 \n"
      "{                           \n"
      "   gl_Position = u_proj * u_view * u_model * vec4(position, 1.0); \n"
      "   vTextureCoords = textCoord;\n"
      "   vFragColor = color;\n"
      "}                           \n";
const char *fragmentShaderSource = 
#ifdef EMSCRIPTEN
      "precision mediump float;                   \n"
#endif
      "uniform sampler2D u_texture;\n"
      "varying vec2 vTextureCoords;\n"
      "varying vec3 vFragColor;\n"
      "void main()                                \n"
      "{                                          \n"
      "  vec4 color =  texture2D(u_texture, vTextureCoords); \n"
      "  color.rgb *=  vFragColor;                \n"
      "  gl_FragColor = color;  \n"
      "}                                          \n";

extern TEngine engine;

void GameObject3DDefaultUpdate(GameObject3D* go) {    
        Camera3D* cam = (Camera3D*) engine.cam2D;    
        
        vec3 cameraUp = {0.0f,1.0f, 0.0f};

        // draw our first triangle
        glUseProgram(go->self.shaderProgram);

        mat4 model = m4_transform(go->transform.position, go->transform.scale, go->transform.rotation);
        unsigned int tMat = glGetUniformLocation(go->self.shaderProgram, "u_model");
        glUniformMatrix4fv(tMat, 1, GL_FALSE, (const float*) &model);
        
        mat4 view = m4_look_at(cam->position, v3_add(cam->position, cam->rotation), cameraUp);
        tMat = glGetUniformLocation(go->self.shaderProgram, "u_view");
        glUniformMatrix4fv(tMat, 1, GL_FALSE, (const float*) &view);
        
        mat4 proj = m4_perspective(engine.width, engine.height, cam->view_angle, cam->view_near, cam->view_distance);
        proj.m[1][1] *= -1;
        tMat = glGetUniformLocation(go->self.shaderProgram, "u_proj");
        glUniformMatrix4fv(tMat, 1, GL_FALSE, (const float*) &proj);
                        
        glUseProgram(0);
}

void GameObject3DDefaultDraw(GameObject3D* go){
        
    glUseProgram(go->self.shaderProgram);

    if(go->image != NULL){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, go->image->texture_id);
    }

    for(int i=0; i < go->graphObj.num_shapes;i++){   
        glBindVertexArrayOES(go->graphObj.shapes[i].VAO);    
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, go->graphObj.shapes[i].iParam.buffer);    
        glDrawElements(GL_TRIANGLES, go->graphObj.shapes[i].iParam.indexesSize, GL_UNSIGNED_INT, 0);

    }
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

void GameObject3DSetShader(GameObject3D *go, const char *vert_source, const char *frag_source){
    
     // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar *const *) &vert_source, NULL);
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
    glShaderSource(fragmentShader, 1, (const GLchar *const *) &frag_source, NULL);
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

void GameObject3DInitDefaultShader(GameObject3D *go){    
    
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

void GameObject3DInitDraw(GameObject3D *go)
{
    if(!(go->self.flags & TIGOR_GAME_OBJECT_FLAG_SHADED))
        return;

    go->self.flags |= TIGOR_GAME_OBJECT_FLAG_INIT;
}

void GameObject3DInitDefault(GameObject3D *go){

    GameObjectShaderInit((GameObject *)go);

    GameObject3DInitDraw(go);
}

void GameObject3DClean(GameObject3D* go){
    //GraphicsObjectClean(&go->graphObj);
}

void GameObject3DRecreate(GameObject3D* go){

    //GameObject3DClean(go);

}

void GameObject3DDestroy(GameObject3D* go){

    //GraphicsObjectDestroy(&go->graphObj);

    if(go->image != NULL)
    {
        if(strlen(go->image->path))
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
    
    glDeleteShader(go->self.shaderProgram);
    glDeleteBuffers(1, &go->graphObj.shapes[0].vParam.buffer);
    glDeleteBuffers(1, &go->graphObj.shapes[0].iParam.buffer);
    glDeleteVertexArraysOES(1, &go->graphObj.shapes[0].VAO);

    go->self.flags &= ~(TIGOR_GAME_OBJECT_FLAG_INIT);
}

void GameObject3DInit(GameObject3D* go, GameObjectType type)
{
    GameObjectSetInitFunc((GameObject *)go, (void *)GameObject3DInitDefault);
    GameObjectSetUpdateFunc((GameObject *)go, (void *)GameObject3DDefaultUpdate);
    GameObjectSetDrawFunc((GameObject *)go, (void *)GameObject3DDefaultDraw);
    GameObjectSetCleanFunc((GameObject *)go, (void *)GameObject3DClean);
    GameObjectSetRecreateFunc((GameObject *)go, (void *)GameObject3DRecreate);
    GameObjectSetDestroyFunc((GameObject *)go, (void *)GameObject3DDestroy);
    
    GameObjectSetShaderInitFunc((GameObject *)go, (void *)GameObject3DInitDefaultShader);

    go->self.obj_type = type;

    Transform3DInit(&go->transform);

    switch(type){
        case TIGOR_GAME_OBJECT_TYPE_3D:
            GraphicsObjectInit(&go->graphObj, TIGOR_VERTEX_TYPE_3D_OBJECT);
            break;
        case TIGOR_GAME_OBJECT_TYPE_PARTICLE_3D:
            GraphicsObjectInit(&go->graphObj, TIGOR_VERTEX_TYPE_3D_PARTICLE);
            break;
        case TIGOR_GAME_OBJECT_TYPE_MODEL:
        case TIGOR_GAME_OBJECT_TYPE_2D:
        case TIGOR_GAME_OBJECT_TYPE_PARTICLE_2D:
            break;
    }

}
