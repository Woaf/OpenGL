#include "light.h"

Light::Light()
{
    color = glm::vec3(1.0f, 1.0f, 1.0f);
    ambientIntensity = 1.0f;
    diffuseIntensity = 0.0;
}

Light::Light(GLfloat shadowWidth, GLfloat shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat difIntensity)
{
    shadowMap = new ShadowMap();
    shadowMap->Init(shadowWidth, shadowHeight);

    color = glm::vec3(red, green, blue);
    ambientIntensity = aIntensity;
    diffuseIntensity = difIntensity;
}

Light::~Light()
{
}
