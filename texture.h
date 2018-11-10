#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <stb_image.h>

class Texture
{
public:
    Texture();
    Texture(char* fileLocation);

    bool loadTexture();
    bool loadTextureA();
    void useTexture();
    void clearTexture();

    ~Texture();

private:
    GLuint texture_id;
    int width, height, bitDepth;

    char* fileLoc;
};

#endif // TEXTURE_H
