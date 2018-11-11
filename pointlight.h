#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <light.h>
#include <omnishadowmap.h>

class PointLight : public Light
{
public:
    PointLight();
    PointLight(GLfloat shadowWidth, GLfloat shadowHeight,
               GLfloat near, GLfloat far,
               GLfloat red, GLfloat green, GLfloat blue,
               GLfloat aIntensity, GLfloat difIntensity,
               GLfloat xPos, GLfloat yPos, GLfloat zPos,
               GLfloat con, GLfloat lin, GLfloat exp);

    void useLight(GLuint ambientIntensityLoc, GLuint ambientColorLoc,
                  GLuint diffuseIntensityLoc, GLuint positionLocation,
                  GLuint constLocation, GLuint linLocation, GLuint expLocation);

    std::vector<glm::mat4> calculateLightTransform();
    GLfloat getFarPlane();

    glm::vec3 getPosition();

    ~PointLight();

protected:
    glm::vec3 position;

    GLfloat constant, linear, exponent;

    GLfloat farPlane;
};

#endif // POINTLIGHT_H
