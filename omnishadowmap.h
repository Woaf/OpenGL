#ifndef OMNISHADOWMAP_H
#define OMNISHADOWMAP_H

#include <shadowmap.h>

class OmniShadowMap : public ShadowMap
{
public:
    OmniShadowMap();

    bool Init(GLuint width, GLuint height);

    void write();

    void read(GLenum textureUnit);

    ~OmniShadowMap();
};

#endif // OMNISHADOWMAP_H
