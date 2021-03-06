QT -= gui
# QMAKE_CXXFLAGS += -Wall -Wextra

CONFIG += c++11 console
CONFIG -= app_bundle


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

LIBS += -lGLEW -lglfw -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -lSDL2 -ldl -lassimp

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    mesh.cpp \
    shader.cpp \
    window.cpp \
    camera.cpp \
    texture.cpp \
    light.cpp \
    material.cpp \
    directionallight.cpp \
    pointlight.cpp \
    spotlight.cpp \
    model.cpp \
    shadowmap.cpp \
    omnishadowmap.cpp \
    skybox.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    mesh.h \
    shader.h \
    window.h \
    camera.h \
    stb_image.h \
    texture.h \
    light.h \
    material.h \
    directionallight.h \
    pointlight.h \
    commonvalues.h \
    spotlight.h \
    model.h \
    shadowmap.h \
    omnishadowmap.h \
    skybox.h

DISTFILES += \
    Resources/shader.vert \
    Resources/shader.frag \
    Resources/SDL_WindowCreation.txt \
    Resources/directional_shadow_map.vert \
    Resources/directional_shadow_map.frag \
    Resources/omni_shadow_map.frag \
    Resources/omni_shadow_map.vert \
    Resources/omni_shadow_map.geom \
    Resources/skybox.vert \
    Resources/skybox.frag
