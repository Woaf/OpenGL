#ifndef LIGHT_H
#define LIGHT_H

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shadowmap.h>

class Light
{
public:
    Light();
    Light(GLfloat shadowWidth, GLfloat shadowHeight,
          GLfloat red, GLfloat green, GLfloat blue,
          GLfloat aIntensity, GLfloat difIntensity);

    ShadowMap* getShadowMap() { return shadowMap; }

    ~Light();

protected:
    glm::vec3 color;
    GLfloat ambientIntensity;
    GLfloat diffuseIntensity;

    glm::mat4 lightProj;

    ShadowMap* shadowMap;
};

#endif // LIGHT_H
