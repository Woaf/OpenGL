#include "window.h"

int Window::Initialise()
{
    if(!glfwInit())
    {
        printf("GLFW initialisation error!");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mainWindow = glfwCreateWindow(width, height, "GLFW WINDOW", nullptr, nullptr);

    if(!mainWindow)
    {
        printf("WINDOW initialisation failed");
        glfwTerminate();
        return 2;
    }

    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    glfwMakeContextCurrent(mainWindow);

    CreateCallbacks();

    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK)
    {
        printf("GLEW initialisation failed!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 3;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, bufferWidth, bufferHeight);

    glfwSetWindowUserPointer(mainWindow, this);

    return 0;
}

GLfloat Window::getXChange()
{
    GLfloat theChange = xChange;
    xChange = 0.0f;

    return theChange;
}

GLfloat Window::getYChange()
{
    GLfloat theChange = yChange;
    yChange = 0.0f;

    return theChange;
}

void Window::CreateCallbacks()
{
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}

void Window::handleKeys(GLFWwindow *window, int key, int code, int action, int mods)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if(key >= 0 && key < 1024)
    {
        if(action == GLFW_PRESS)
        {
            theWindow->keys[key] = true;
        }
        else if(action == GLFW_RELEASE)
        {
            theWindow->keys[key] = false;
        }
    }
}

void Window::handleMouse(GLFWwindow *window, double xPos, double yPos)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if(theWindow->mouseFirstMoved)
    {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMoved = false;
    }

    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos;

    theWindow->lastX = xPos;
    theWindow->lastY = yPos;
}
