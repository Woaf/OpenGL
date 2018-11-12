#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <mesh.h>
#include <shader.h>

#include <commonvalues.h>

class Skybox
{
public:
    Skybox();

    Skybox(std::vector<std::string> faceLocations);

    void drawSkybox(glm::mat4 viewMatrix, glm::mat4 projMatrix);

    ~Skybox();

private:
    Mesh* skyMesh;
    Shader* skyShader;

    GLuint textureId;
    GLuint uniformProj, uniformView;
};

#endif // SKYBOX_H
