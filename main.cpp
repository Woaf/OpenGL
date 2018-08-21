#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>

#include <stdio.h>

#include <string>

const GLint WIDTH = 1280, HEIGHT = 720;
const GLint START_X = 100, START_Y = 100;

static bool quit = false;

static GLuint VAO, VBO, shader;

void WriteErrorMessage(const char* msg)
{
    fprintf(stderr, "%s\n", msg);
}

void exitCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        quit = true;
}

static const char* vShader = "#version  330\n\
layout (location = 0) in vec3 pos;\n\
void main()\n\
{\n\
    gl_Position = vec4(pos, 1.0f);\n\
}";

static const char* fShader = "#version 330\n\
out vec4 col;\n\
void main()\n\
{\n\
    col = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n\
}";

void CreateTriangle()
{
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if(!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        WriteErrorMessage(eLog);
        return;
    }
    glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
    shader = glCreateProgram();

    if(!shader)
    {
        WriteErrorMessage("Error creating the shader program");
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if(!result)
    {
        glGetProgramInfoLog(shader, sizeof (eLog), NULL, eLog);
        WriteErrorMessage("Error creating shader");
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if(!result)
    {
        glGetProgramInfoLog(shader, sizeof (eLog), NULL, eLog);
        WriteErrorMessage("Error creating shader");
        return;
    }
}

int main()
{

    /*
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        WriteErrorMessage("SLD initialisation failed!");
        return -1;
    }

    SDL_Window *mainWindow = SDL_CreateWindow("SDL Window",
                                       START_X,
                                       START_Y,
                                       WIDTH,
                                       HEIGHT,
                                       SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if(!mainWindow)
    {
        WriteErrorMessage("WINDOW initialisation failed!");
        SDL_Quit();
        return -2;
    }

    int bufferWidth, bufferHeight;

    SDL_GetWindowSize(mainWindow, &bufferWidth, &bufferHeight);

    SDL_GLContext context = SDL_GL_CreateContext(mainWindow);

    if(!context)
    {
        WriteErrorMessage("Context creation error");
        return -3;
    }

    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK)
    {
        WriteErrorMessage("GLEW initialisation failed");
        return -4;
    }

    glViewport(0, 0, bufferWidth, bufferHeight);

    bool quit = false;
    SDL_Event event;

    while(!quit)
    {

        while(SDL_PollEvent(&event))
        {
            switch (event.type) {
                case(SDL_QUIT):
                    quit = true;
                    break;
                case(SDL_KEYDOWN):
                    if(event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        quit = true;
                    }
                break;

            }
        }

        glClearColor(0.2f, 0.5f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(mainWindow);
    }

    SDL_DestroyWindow(mainWindow);
    SDL_Quit();

    */

    if(!glfwInit())
    {
        WriteErrorMessage("GLFW initialisation failed!");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "GLFW WINDOW", nullptr, nullptr);

    if(!mainWindow)
    {
        WriteErrorMessage("WINDOW initialisation failed!");
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

    glViewport(0, 0, bufferWidth, bufferHeight);

    glfwSetKeyCallback(mainWindow, exitCallback);

    CreateTriangle();
    CompileShaders();

    while(!glfwWindowShouldClose(mainWindow) && !quit)
    {
        glfwPollEvents();


        glClearColor(0.3f, 0.5f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glUseProgram(0);
        glfwSwapBuffers(mainWindow);
    }

    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 0;
}
