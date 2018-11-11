#include "pointlight.h"

PointLight::PointLight() : Light()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    constant = 1.0f;
    linear = 0.0f;
    exponent = 0.0f;
}

PointLight::PointLight(GLfloat shadowWidth, GLfloat shadowHeight,
                       GLfloat near, GLfloat far,
                       GLfloat red, GLfloat green, GLfloat blue,
                       GLfloat aIntensity, GLfloat difIntensity,
                       GLfloat xPos, GLfloat yPos, GLfloat zPos,
                       GLfloat con, GLfloat lin, GLfloat exp) : Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, difIntensity)
{
    position = glm::vec3(xPos, yPos, zPos);
    constant = con;
    linear = lin;
    exponent = exp;

    float aspectRatio = (float)shadowWidth / (float)shadowHeight;

    farPlane = far;
    lightProj = glm::perspective(glm::radians(90.0f), aspectRatio, near, far);

    shadowMap = new OmniShadowMap();
    shadowMap->Init(shadowWidth, shadowHeight);
}

void PointLight::useLight(GLuint ambientIntensityLoc, GLuint ambientColorLoc,
                          GLuint diffuseIntensityLoc, GLuint positionLocation,
                          GLuint constLocation, GLuint linLocation, GLuint expLocation)
{
    glUniform3f(ambientColorLoc, color.x, color.y, color.z);
    glUniform1f(ambientIntensityLoc, ambientIntensity);
    glUniform1f(diffuseIntensityLoc, diffuseIntensity);

    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform1f(constLocation, constant);
    glUniform1f(linLocation, linear);
    glUniform1f(expLocation, exponent);
}

std::vector<glm::mat4> PointLight::calculateLightTransform()
{
    std::vector<glm::mat4> lightMatrices;
    lightMatrices.push_back(lightProj * glm::lookAt(position, position+glm::vec3(1.0, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    lightMatrices.push_back(lightProj * glm::lookAt(position, position+glm::vec3(-1.0, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    lightMatrices.push_back(lightProj * glm::lookAt(position, position+glm::vec3(0.0, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    lightMatrices.push_back(lightProj * glm::lookAt(position, position+glm::vec3(0.0, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    lightMatrices.push_back(lightProj * glm::lookAt(position, position+glm::vec3(0.0, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    lightMatrices.push_back(lightProj * glm::lookAt(position, position+glm::vec3(0.0, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

    return lightMatrices;
}

GLfloat PointLight::getFarPlane()
{
    return farPlane;
}

glm::vec3 PointLight::getPosition()
{
    return position;
}

PointLight::~PointLight()
{

}
