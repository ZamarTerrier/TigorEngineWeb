#include "e_texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "gameObject2D.h"


void TextureLoadImage(GameObject *go, const char *filePath){

    GameObject2D *go2D = (GameObject2D *) go;
    
        int width, height, nrChannels;
        char *pixels = stbi_load(filePath, &width, &height, &nrChannels, 0);

        go2D->image->imgWidth = width;
        go2D->image->imgHeight = height;

            // Determine GL texture format
        int format = -1;
        if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
            
        printf ("Image dimensions %dx%d, %d bits per pixel\n", width, height, nrChannels * 8);

        glGenTextures(1, &go2D->image->texture_id);
        glBindTexture(GL_TEXTURE_2D, go2D->image->texture_id);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);

        stbi_image_free(pixels);
        
        glBindTexture(GL_TEXTURE_2D, 0);
}