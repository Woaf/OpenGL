#include "directionallight.h"

DirectionalLight::DirectionalLight() : Light()
{
    direction = glm::vec3(0.0f, -1.0f, 0.0f);
    lightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f,
                           0.0f, 100.0f);
}

DirectionalLight::DirectionalLight(GLfloat shadowWidth, GLfloat shadowHeight,
                                   GLfloat red, GLfloat green, GLfloat blue,
                                   GLfloat aIntensity, GLfloat difIntensity,
                                   GLfloat xDir, GLfloat yDir, GLfloat zDir)
                    : Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, difIntensity)
{
    direction = glm::vec3(xDir, yDir, zDir);
    lightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f,
                           0.0f, 100.0f);
}

void DirectionalLight::useLight(GLuint ambientIntensityLoc, GLuint ambientColorLoc,
                     GLuint diffuseIntensityLoc, GLuint directionLocation)
{
    glUniform3f(ambientColorLoc, color.x, color.y, color.z);
    glUniform1f(ambientIntensityLoc, ambientIntensity);

    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(diffuseIntensityLoc, diffuseIntensity);
}

glm::mat4 DirectionalLight::calculateLightTransform()
{
    return lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

DirectionalLight::~DirectionalLight()
{

}
