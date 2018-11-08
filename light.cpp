#include "light.h"

Light::Light()
{
    color = glm::vec3(1.0f, 1.0f, 1.0f);
    ambientIntensity = 1.0f;

    direction = glm::vec3(0.0f, -1.0f, 0.0f);
    diffuseIntensity = 0.0;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
             GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat difIntensity)
{
    color = glm::vec3(red, green, blue);
    ambientIntensity = aIntensity;

    direction = glm::vec3(xDir, yDir, zDir);
    diffuseIntensity = difIntensity;
}

void Light::useLight(GLuint ambientIntensityLoc, GLuint ambientColorLoc,
                     GLuint diffuseIntensityLoc, GLuint directionLocation)
{
    glUniform3f(ambientColorLoc, color.x, color.y, color.z);
    glUniform1f(ambientIntensityLoc, ambientIntensity);

    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(diffuseIntensityLoc, diffuseIntensity);
}

Light::~Light()
{
}
