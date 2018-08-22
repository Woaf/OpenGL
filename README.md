# OpenGL
A project to hold what I have learned in a Udemy online course

## Installing GLFW

To install GLFW on your system, follow the following steps from: http://web.eecs.umich.edu/~sugih/courses/eecs487/glut-howto/glfw/

```
First install GLFW's dependencies, including OpenGL/Mesa:

sudo apt-get install cmake xorg-dev libglu1-mesa-dev
You must have sudo privileges. (On Red Hat, you'll need to install cmake28 instead.)

You should now have:

/usr/include/GL
/usr/lib/x86_64-linux-gnu/libGL.so

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

## Installing SDL (and GLEW, if your QT doesn't have it for some reason)
```
sudo apt-get install libsdl2-2.0
sudo apt-get install libsdl2-dev
sudo apt-get install libglew-dev
```
## Linking the libraries in the QMake file
Add the following line to your .pro file.
```
LIBS += -lGLEW -lglfw -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -lSDL2
```
These libraries both include GLEW, GLFW, and SDL2.

## Install GLM libraries
All you have to do to use the GLM packages, is to install GLM via Terminal.
`sudo apt-get install libglm-dev`
I think, if you also want the documentation for it, then you can install `sudo apt-get install libglm-doc` as well, but I'm not sure about this.

## Your set-up is ready! 
You can begin coding now :) 

## Bugs that I don't udnerstand
For some reason, linking "-lglfw3" did not seem to work, although it should be that. Instead, removing the "3" from the end seemed to solve the problem.
