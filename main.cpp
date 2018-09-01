#include <stdio.h>
#include <cmath>
#include <string>
#include <vector>

#include <window.h>
#include <mesh.h>
#include <shader.h>
#include <camera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static Window mainWindow;
static Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

static std::vector<Mesh*> meshList;
static std::vector<Shader*> shaderList;

static const char* vShader = "../OpenGL/Resources/shader.vert";

static const char* fShader = "../OpenGL/Resources/shader.frag";

void createObjects()
{
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, -1.0, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    Mesh *obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj1);
}

void CreateShaders()
{
    Shader *shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(shader1);
}

int main()
{
    mainWindow = Window(2000, 720);
    mainWindow.Initialise();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

    createObjects();
    CreateShaders();

    glm::mat4 proj(1.0f);
    GLfloat near = 0.001f;
    GLfloat far = 10.0f;
    proj = glm::perspective(45.0f,
                            mainWindow.getBufferWidth()/mainWindow.getBufferHeight(),
                            near,
                            far);

    GLint uniformProj = 0;
    GLint uniformModel = 0;
    GLint uniformView = 0;


    float x = 0.0f;

    while(!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        x += 0.01f;

        glfwPollEvents();
        camera.keyControl(mainWindow.getKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        glClearColor(0.2275f, 0.251f, 0.3530f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0]->UseShader();
        uniformModel = shaderList[0]->GetModelLocation();
        uniformProj = shaderList[0]->GetProjectionLocation();
        uniformView = shaderList[0]->GetViewLocation();

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
        model = glm::scale(model, glm::vec3(0.4f));
        model = glm::rotate(model, x, glm::vec3(1.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProj, 1, GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

        meshList[0]->RenderMesh();

        glUseProgram(0);

        mainWindow.swapBuffers();
    }

    return 0;
}
