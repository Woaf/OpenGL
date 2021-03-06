#include "spotlight.h"

SpotLight::SpotLight() : PointLight ()
{
    direction = glm::vec3(0.0f, -1.0f, 0.0f);
    edge = 0.0f;
    procEdge = cosf(glm::radians(edge));
    isOn = true;
}

SpotLight::SpotLight(GLfloat shadowWidth, GLfloat shadowHeight,
                     GLfloat near, GLfloat far,
                     GLfloat red, GLfloat green, GLfloat blue,
                     GLfloat aIntensity, GLfloat difIntensity,
                     GLfloat xPos, GLfloat yPos, GLfloat zPos,
                     GLfloat xDir, GLfloat yDir, GLfloat zDir,
                     GLfloat con, GLfloat lin, GLfloat exp,
                     GLfloat edg) : PointLight (shadowWidth, shadowHeight, near, far, red, green, blue, aIntensity, difIntensity, xPos, yPos, zPos, con, lin, exp)
{
    direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
    edge = edg;
    procEdge = cosf(glm::radians(edge));
    isOn = true;
}

void SpotLight::useLight(GLuint ambientIntensityLoc, GLuint ambientColorLoc,
                         GLuint diffuseIntensityLoc, GLuint positionLocation,
                         GLuint directionLocation,
                         GLuint constLocation, GLuint linLocation, GLuint expLocation,
                         GLuint edgeLocaion)
{
    glUniform3f(ambientColorLoc, color.x, color.y, color.z);

    if(isOn)
    {
        glUniform1f(ambientIntensityLoc, ambientIntensity);
        glUniform1f(diffuseIntensityLoc, diffuseIntensity);
    }
    else
    {
        glUniform1f(ambientIntensityLoc, 0.0f);
        glUniform1f(diffuseIntensityLoc, 0.0f);
    }

    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform1f(constLocation, constant);
    glUniform1f(linLocation, linear);
    glUniform1f(expLocation, exponent);

    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(edgeLocaion, procEdge);
}

void SpotLight::setFlash(glm::vec3 pos, glm::vec3 dir)
{
    position = pos;
    direction = dir;
}

SpotLight::~SpotLight()
{

}
