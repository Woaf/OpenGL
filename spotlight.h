#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include <pointlight.h>

class SpotLight : public PointLight
{
public:
    SpotLight();

    SpotLight(GLfloat shadowWidth, GLfloat shadowHeight,
              GLfloat near, GLfloat far,
              GLfloat red, GLfloat green, GLfloat blue,
              GLfloat aIntensity, GLfloat difIntensity,
              GLfloat xPos, GLfloat yPos, GLfloat zPos,
              GLfloat xDir, GLfloat yDir, GLfloat zDir,
              GLfloat con, GLfloat lin, GLfloat exp,
              GLfloat edg);

    void useLight(GLuint ambientIntensityLoc, GLuint ambientColorLoc,
                  GLuint diffuseIntensityLoc, GLuint positionLocation, GLuint directionLocation,
                  GLuint constLocation, GLuint linLocation, GLuint expLocation, GLuint edgeLocaion);

    void setFlash(glm::vec3 pos, glm::vec3 dir);

    ~SpotLight();

private:
    glm::vec3 direction;

    GLfloat edge, procEdge;
};

#endif // SPOTLIGHT_H
