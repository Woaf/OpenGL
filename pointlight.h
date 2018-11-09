#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <light.h>

class PointLight : public Light
{
public:
    PointLight();
    PointLight(GLfloat red, GLfloat green, GLfloat blue,
               GLfloat aIntensity, GLfloat difIntensity,
               GLfloat xPos, GLfloat yPos, GLfloat zPos,
               GLfloat con, GLfloat lin, GLfloat exp);

    void useLight(GLuint ambientIntensityLoc, GLuint ambientColorLoc,
                  GLuint diffuseIntensityLoc, GLuint positionLocation,
                  GLuint constLocation, GLuint linLocation, GLuint expLocation);

    ~PointLight();

private:
    glm::vec3 position;

    GLfloat constant, linear, exponent;
};

#endif // POINTLIGHT_H
