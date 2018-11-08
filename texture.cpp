#include "texture.h"

Texture::Texture()
{
    texture_id = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLoc = "";
}

Texture::Texture(char *fileLocation)
{
    texture_id = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLoc = fileLocation;
}

void Texture::loadTexture()
{
    unsigned char* textureData= stbi_load(fileLoc, &width, &height, &bitDepth, 0);

    if(!textureData)
    {
        printf("Could not load image from %s", fileLoc);
        return;
    }

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(textureData);
}

void Texture::useTexture()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

void Texture::clearTexture()
{
    glDeleteTextures(1, &texture_id);

    texture_id = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLoc = "";
}

Texture::~Texture()
{
    clearTexture();
}