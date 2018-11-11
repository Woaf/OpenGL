#ifndef OMNISHADOWMAP_H
#define OMNISHADOWMAP_H

#include <shadowmap.h>

class OmniShadowMap : public ShadowMap
{
public:
    OmniShadowMap();

    bool Init(unsigned int width, unsigned int height);

    void write();

    void read(GLenum textureUnit);

    ~OmniShadowMap();
};

#endif // OMNISHADOWMAP_H
