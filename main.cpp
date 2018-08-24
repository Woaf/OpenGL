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

const GLint WIDTH = 1280, HEIGHT = 720;
const GLint START_X = 100, START_Y = 100;

static bool quit = false;

static GLuint shader, uniformModel, uniformProj;

float xMove = 0.0f, yMove = 0.0f;

static float IN_RADIANS = M_PI / 180.0f;

static std::vector<Mesh*> meshList;

void WriteErrorMessage(const char* msg)
{
    fprintf(stderr, "%s\n", msg);
}

void exitCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        quit = true;
}

static const char* vShader = "#version 330\n\
layout (location = 0) in vec3 pos;\n\
uniform mat4 model;\n\
uniform mat4 proj;\n\
out vec4 vCol;\n\
void main()\n\
{\n\
    gl_Position = proj * model * vec4(pos, 1.0f);\n\
    vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);\n\
}";

static const char* fShader = "#version 330\n\
out vec4 col;\n\
in vec4 vCol;\n\
void main()\n\
{\n\
    col = vCol;\n\
}";

void createTriangle()
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

void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar elog[1024] = {0};

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if(!result)
    {
        glGetShaderInfoLog(theShader, sizeof(elog), nullptr, elog);
        WriteErrorMessage("ERROR linking shader!");
        return;
    }

    glAttachShader(theProgram, theShader);
}

void compileShader()
{
    shader = glCreateProgram();

    if(!shader)
    {
        WriteErrorMessage("Error creating SHADER PROGRAM!");
        return;
    }

    addShader(shader, vShader, GL_VERTEX_SHADER);
    addShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar elog[1024] = {0};

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if(!result)
    {
        glGetProgramInfoLog(shader, sizeof(elog), nullptr, elog);
        WriteErrorMessage("ERROR linking shader!");
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if(!result)
    {
        glGetProgramInfoLog(shader, sizeof(elog), nullptr, elog);
        WriteErrorMessage("ERROR validating shader!");
        return;
    }

    uniformModel = glGetUniformLocation(shader, "model");
    uniformModel = glGetUniformLocation(shader, "proj");
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

    createTriangle();
    compileShader();

    glm::mat4 proj = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.0f, 50.0f);

    while(!glfwWindowShouldClose(mainWindow) && !quit)
    {
        glfwPollEvents();

        xMove += 0.0005f;
        yMove += 0.0005f;

        glClearColor(0.2f, 0.5f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(sin(0), 0.0f, 0.0f));
        model = glm::rotate(model, xMove * 3.6f * IN_RADIANS, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.8f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProj, 1, GL_FALSE, glm::value_ptr(model));

        meshList[0]->RenderMesh();

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 0;
}
