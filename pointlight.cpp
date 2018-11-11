#include "pointlight.h"

PointLight::PointLight() : Light()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    constant = 1.0f;
    linear = 0.0f;
    exponent = 0.0f;
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue,
                       GLfloat aIntensity, GLfloat difIntensity,
                       GLfloat xPos, GLfloat yPos, GLfloat zPos,
                       GLfloat con, GLfloat lin, GLfloat exp) : Light(1024, 1024, red, green, blue, aIntensity, difIntensity)
{
    position = glm::vec3(xPos, yPos, zPos);
    constant = con;
    linear = lin;
    exponent = exp;
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

PointLight::~PointLight()
{

}
