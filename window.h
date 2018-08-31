#ifndef WINDOW_H
#define WINDOW_H

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window() : width (720), height(400) {}
    Window(GLint windowWidth, GLint windowHeight) : width(windowWidth), height(windowHeight) {}

    int Initialise();
    GLFWwindow* getWindow() {return mainWindow;}
    GLfloat getBufferWidth() {return bufferWidth;}
    GLfloat getBufferHeight() {return bufferHeight;}
    bool getShouldClose() {return glfwWindowShouldClose(mainWindow);}
    void swapBuffers() { glfwSwapBuffers(mainWindow);}

    ~Window() { glfwDestroyWindow(mainWindow); glfwTerminate(); }
private:
    GLFWwindow* mainWindow;

    GLint width, height, bufferWidth, bufferHeight;
};

#endif // WINDOW_H
