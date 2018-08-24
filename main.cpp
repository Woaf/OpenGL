#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <cmath>
#include <string>
#include <vector>

#include <mesh.h>
#include <shader.h>

const GLint WIDTH = 1280, HEIGHT = 720;
const GLint START_X = 100, START_Y = 100;

static bool quit = false;

float xMove = 0.0f, yMove = 0.0f;

static float IN_RADIANS = M_PI / 180.0f;

static std::vector<Mesh*> meshList;
static std::vector<Shader*> shaderList;

void WriteErrorMessage(const char* msg)
{
    fprintf(stderr, "%s\n", msg);
}

void exitCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        quit = true;
}

static const char* vShader = "../OpenglCourse/Resources/shader.vert";

static const char* fShader = "../OpenglCourse/Resources/shader.frag";

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

    Mesh *obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj2);

}

void CreateShaders()
{
    Shader *shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(shader1);
}

int main()
{

    if(!glfwInit())
    {
        WriteErrorMessage("GLFW initialisation error!");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "GLFW WINDOW", nullptr, nullptr);

    if(!mainWindow)
    {
        WriteErrorMessage("WINDOW initialisation failed");
        glfwTerminate();
        return 2;
    }

    int bufferWidth, bufferHeight;

    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    glfwMakeContextCurrent(mainWindow);

    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK)
    {
        WriteErrorMessage("GLEW initialisation failed!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 3;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, bufferWidth, bufferHeight);

    glfwSetKeyCallback(mainWindow, exitCallback);

    createObjects();
    CreateShaders();

    glm::mat4 proj = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.0f, 50.0f);

    GLuint uniformProj = 0, uniformModel = 0;

    while(!glfwWindowShouldClose(mainWindow) && !quit)
    {
        glfwPollEvents();

        xMove += 0.0005f;
        yMove += 0.0005f;

        glClearColor(0.2f, 0.5f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0]->UseShader();
        uniformModel = shaderList[0]->GetModelLocation();
        uniformProj = shaderList[0]->GetProjectionLocation();

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(sin(0), 0.0f, 0.0f));
        model = glm::rotate(model, xMove * 3.6f * IN_RADIANS, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.8f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProj, 1, GL_FALSE, glm::value_ptr(model));

        meshList[0]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, sin(yMove), 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        meshList[1]->RenderMesh();

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 0;
}
