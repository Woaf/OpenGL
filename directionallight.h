#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include <light.h>

class DirectionalLight : public Light
{
public:
    DirectionalLight();
    DirectionalLight( GLfloat shadowWidth, GLfloat shadowHeight,
                     GLfloat red, GLfloat green, GLfloat blue,
                     GLfloat aIntensity, GLfloat difIntensity,
                     GLfloat xDir, GLfloat yDir, GLfloat zDir);

    void useLight(GLuint ambientIntensityLoc, GLuint ambientColorLoc,
                  GLuint diffuseIntensityLoc, GLuint directionLocation);

    glm::mat4 calculateLightTransform();

    ~DirectionalLight();

private:
    glm::vec3 direction;
};

#endif // DIRECTIONALLIGHT_H
