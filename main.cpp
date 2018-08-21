#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>

#include <stdio.h>
#include <iostream>

const GLint WIDTH = 1280, HEIGHT = 720;
const GLint START_X = 100, START_Y = 100;

void WriteErrorMessage(const char* msg)
{
    fprintf(stderr, "%s\n", msg);
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

    while(!glfwWindowShouldClose(mainWindow) || quit)
    {
        glfwPollEvents();

        glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(mainWindow);
    }

    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 0;
}
