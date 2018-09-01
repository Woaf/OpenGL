#ifndef WINDOW_H
#define WINDOW_H

#include <stdio.h>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window() : width (720), height(400) { for(size_t i = 0; i < 1024; i++) {keys[i] = 0;} }
    Window(GLint windowWidth, GLint windowHeight) : width(windowWidth), height(windowHeight) { for(size_t i = 0; i < 1024; i++) {keys[i] = 0;} }

    int Initialise();
    GLFWwindow* getWindow() {return mainWindow;}
    GLfloat getBufferWidth() {return bufferWidth;}
    GLfloat getBufferHeight() {return bufferHeight;}
    bool getShouldClose() {return glfwWindowShouldClose(mainWindow);}
    void swapBuffers() { glfwSwapBuffers(mainWindow);}

    bool* getKeys() {return keys;}
    GLfloat getXChange();
    GLfloat getYChange();

    ~Window() { glfwDestroyWindow(mainWindow); glfwTerminate(); }
private:
    GLFWwindow* mainWindow;

    GLint width, height, bufferWidth, bufferHeight;

    bool keys[1024];
    GLfloat lastX, lastY, xChange, yChange;
    bool mouseFirstMoved;

    void CreateCallbacks();
    static void handleKeys(GLFWwindow* window,
                           int key,
                           int code,
                           int action,
                           int mods);
    static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};

#endif // WINDOW_H
