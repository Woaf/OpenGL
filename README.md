# OpenGL
A project, to show what I have learned in a Udemy online course

## Installing GLFW
GLFW and SDL are libraries that help creating a context (or a window) that you can draw OpenGL stuff in. Both of them are very useful, but make sure to use only one of them. Esentially, both of these libraries help you not only create a window for OpenGL, but they also handle user inputs, so you can interact with your graphics application.

To install GLFW on your system, follow the following steps from: http://web.eecs.umich.edu/~sugih/courses/eecs487/glut-howto/glfw/

```
First install GLFW's dependencies, including OpenGL/Mesa:

sudo apt-get install cmake xorg-dev libglu1-mesa-dev
You must have sudo privileges. (On Red Hat, you'll need to install cmake28 instead.)

You should now have:

/usr/include/GL
/usr/lib/x86_64-linux-gnu/libGL.so

[WOAF EDIT: Now, you should download the zipped GLFW package from its official website. You can download and unzip it anywhere on your computer. The following steps require you to install GLFW from the unzipped folder. After the installation, the downloaded zip and the unzipped package is no longer required.]

At the time of writing, the GLFW package one gets with apt-get is version 2.7 whereas we're using version 3.0.4. So we need to install GLFW from source. Unzip the source file and change your working directory to the glfw-3.0.4 directory. Install GLFW:

cd glfw-3.0.4
rehash
cmake -G "Unix Makefiles"
make
sudo make install

You should now have:
/usr/local/include/GLFW
/usr/local/lib/libglfw3.a

In your OpenGL source files, include the following line:

#include <GLFW/glfw3.h>

You don't need to include gl.h as it is already included in glfw3.h.
For an example, see the provided sample source code.

If you want to include glu.h automatically, set the -DGLFW_INCLUDE_GLU compiler flag.

To make a GLFW application on the command line, use the following linker options:

-lglfw3 -lGL -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread

The last three libraries are needed on Ubuntu 14.04.1.
```
To install it on other operating systems, visit the link above.

## Installing SDL and GLEW
SDL knows a whole bunch more than GLFW, but its also a bit more complicated to use (in my opinion). To demonstrate the difference, I will leave the SDL implementation in a Txt file within the Resources folder. For begginers, I would recommend using GLFW, and once they want to move onto more comlicated stuff, use SDL.

GLEW is a library that helps you write OpenGL code. Remember, OpenGL is not a package nor a library - but its a specification of HOW code should be written and passed on to your GPU. GLEW is "the bridge" between your CPU and your GPU. (But do NOT quote me on that one! :D )

To install:
```
sudo apt-get install libsdl2-2.0
sudo apt-get install libsdl2-dev
sudo apt-get install libglew-dev
```
## Linking the libraries in the QMake file
Add the following line to your .pro file.
```
LIBS += -lGLEW -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -lSDL2 -ldl
```
These libraries both include GLEW, GLFW, and SDL2.

## Install GLM libraries
GLM stands for "Graphics Language Maths" and it is a library filled with mathematical structures along with their operations. This library helps you create vectros, matrices, and whatnot so you do not have to define these operations yourself.

All you have to do to use the GLM packages, is to install GLM via Terminal `sudo apt-get install libglm-dev`.

I think, if you also want the documentation for it, then you can `sudo apt-get install libglm-doc` as well, but I'm not sure about this.

## Your set-up is ready! 
You can begin coding now :) 

## Bugs that I don't udnerstand
For some reason, linking "-lglfw3" did not seem to work, although it should be that. Instead, removing the "3" from the end seemed to solve the problem. 

[EDIT: I later installed the environment on my PC, and it worked with `-lglfw3`, so I guess if you seem to have a library linking error, then the first thing you should try is either removing or adding "3" at the end of the glfw linkage. Also, I found that if you seem to have an error with some symbols in a .so library file while building the project, then adding `-ldl` to the linking libraries solves the problem.]
