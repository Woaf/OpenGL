#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include <stdio.h>
#include <GL/glew.h>

class ShadowMap
{
public:
    ShadowMap();

    virtual bool Init(GLuint width, GLuint height);

    virtual void write();

    virtual void read(GLenum textureUnit);

    GLuint getShadowWidth() { return shadowWidth; }
    GLuint getShadowHeight() { return shadowHeight; }

    ~ShadowMap();

private:
    GLuint FBO, shadowMap;
    GLuint shadowWidth, shadowHeight;
};

#endif // SHADOWMAP_H