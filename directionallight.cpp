#include "directionallight.h"

DirectionalLight::DirectionalLight() : Light()
{
    direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
                                   GLfloat aIntensity, GLfloat difIntensity,
                                   GLfloat xDir, GLfloat yDir, GLfloat zDir)
                    : Light(red, green, blue, aIntensity, difIntensity)
{
    direction = glm::vec3(xDir, yDir, zDir);
}

void DirectionalLight::useLight(GLuint ambientIntensityLoc, GLuint ambientColorLoc,
                     GLuint diffuseIntensityLoc, GLuint directionLocation)
{
    glUniform3f(ambientColorLoc, color.x, color.y, color.z);
    glUniform1f(ambientIntensityLoc, ambientIntensity);

    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(diffuseIntensityLoc, diffuseIntensity);
}

DirectionalLight::~DirectionalLight()
{

}
