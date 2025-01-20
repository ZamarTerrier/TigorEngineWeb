#include "e_texture.h"

#include "e_texture_variables.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "gameObject2D.h"

extern TEngine engine;

uint32_t TextureFindTexture(const char *path, int *width, int *height)
{
    if(path == NULL)
        return 0;

    engine_buffered_image *buff_images = engine.DataR.e_var_images;

    char *temp;
    for(int i=0;i < engine.DataR.e_var_num_images;i++)
    {
        temp = buff_images[i].path;
        if(!strcmp(temp, path)){
            *width = buff_images[i].texture.image_data.texWidth;
            *height = buff_images[i].texture.image_data.texHeight;
            return buff_images[i].buffer;
        }
    }

    return 0;
}


void TextureLoadImage(GameObject *go, const char *filePath){

    GameObject2D *go2D = (GameObject2D *) go;
    
    engine_buffered_image *images = engine.DataR.e_var_images;
    
    uint32_t temp_tex = TextureFindTexture(filePath, &go2D->image->imgWidth, &go2D->image->imgHeight);

    if(temp_tex != 0)
    {
        go2D->image->texture_id = temp_tex;
        return;
    }
    
    if(engine.DataR.e_var_num_images > MAX_IMAGES - 2)
    {
        printf("Too much images!\n");        
        go2D->image->texture_id = images[MAX_IMAGES - 1].buffer;
        go2D->image->imgWidth = images[MAX_IMAGES - 1].texture.image_data.texWidth;
        go2D->image->imgHeight = images[MAX_IMAGES - 1].texture.image_data.texHeight;
        return;
    }

    int len = strlen(filePath);
    memset(images[engine.DataR.e_var_num_images].path, 0, 256);
    memcpy(images[engine.DataR.e_var_num_images].path, filePath, len);

    int width, height, nrChannels;
    stbi_uc *pixels = stbi_load(filePath, &width, &height, &nrChannels, 0);

    go2D->image->imgWidth = width;
    go2D->image->imgHeight = height;
    images[engine.DataR.e_var_num_images].texture.image_data.texWidth = width;
    images[engine.DataR.e_var_num_images].texture.image_data.texHeight = height;

    // Determine GL texture format
    int format = -1;
    if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;
            
    printf ("Image dimensions %dx%d, %d bits per pixel\n", width, height, nrChannels * 8);

    glGenTextures(1, &images[engine.DataR.e_var_num_images].buffer);
    glBindTexture(GL_TEXTURE_2D, images[engine.DataR.e_var_num_images].buffer);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
    
    stbi_image_free(pixels);
        
    glBindTexture(GL_TEXTURE_2D, 0);
    
    go2D->image->texture_id = images[engine.DataR.e_var_num_images].buffer;

    engine.DataR.e_var_num_images ++;
}