#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string>
#include <cmath>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <commonvalues.h>

#include <assimp/Importer.hpp>

#include <window.h>
#include <mesh.h>
#include <shader.h>
#include <camera.h>
#include <texture.h>
#include <directionallight.h>
#include <pointlight.h>
#include <spotlight.h>
#include <material.h>
#include  <model.h>

static Window mainWindow;
static Camera camera;

static Texture brickTexture;
static Texture groundTexture;

static DirectionalLight mainLight;
static PointLight pointLights[MAX_POINT_LIGHTS];
static SpotLight spotLights[MAX_SPOT_LIGHTS];

static Material shinyMaterial;
static Material dullMaterial;

Model bmw;
Model wolf;
Model cat;

static GLfloat deltaTime = 0.0f;
static GLfloat lastTime = 0.0f;

static std::vector<Mesh*> meshList;
static std::vector<Shader> shaderList;

static const char* vShader = "../OpenGL/Resources/shader.vert";

static const char* fShader = "../OpenGL/Resources/shader.frag";

void calcAverageNormals(unsigned int* indices, unsigned int indexCount,
                        GLfloat* vertices, unsigned int vertexCount,
                        unsigned int vLength, unsigned int normalOffset)
{
    glm::vec3 v1(0.0f);
    glm::vec3 v2(1.0f);
    glm::vec3 normal(-1.0f);
    for(size_t i = 0; i < indexCount; i += 3)
    {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i+1] * vLength;
        unsigned int in2 = indices[i+2] * vLength;

        v1 = glm::vec3(vertices[in1] - vertices[in0],
                     vertices[in1+1] - vertices[in0+1],
                     vertices[in1+2] - vertices[in0+2]);

        v2 = glm::vec3(vertices[in2] - vertices[in0],
                     vertices[in2+1] - vertices[in0+1],
                     vertices[in2+2] - vertices[in0+2]);

        normal = glm::normalize(glm::cross(v1, v2));

        in0 += normalOffset;
        in1 += normalOffset;
        in2 += normalOffset;

        vertices[in0] += normal.x;
        vertices[in0+1] += normal.y;
        vertices[in0+2] += normal.z;

        vertices[in1] += normal.x;
        vertices[in1+1] += normal.y;
        vertices[in1+2] += normal.z;

        vertices[in2] += normal.x;
        vertices[in2+1] += normal.y;
        vertices[in2+2] += normal.z;
    }

    for(size_t i = 0; i < vertexCount / vLength; i++)
    {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset+1], vertices[nOffset+2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x; vertices[nOffset+1] = vec.y; vertices[nOffset+2] = vec.z;
    }
}

void createObjects()
{
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
        -1.0f, -1.0f, -0.6f,   0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
         0.0f, -1.0f, 1.0f,    0.5f, 0.0f,   0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -0.6f,   1.0f, 0.0f,   0.0f, 0.0f, 0.0f,
         0.0f,  1.0f, 0.0f,    0.5f, 1.0f,   0.0f, 0.0f, 0.0f,
    };

    unsigned int floor_indices[] = {
        0, 2, 1,
        1, 2, 3
    };

    GLfloat floor_vertices[] = {
        -10.0f, 0.0f, -10.f,  0.0f, 0.0f,   0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, -10.0f,  2.0f, 0.0f,   0.0f, -1.0f, 0.0f,
        -10.0f, 0.0f, 10.0f,  0.0f, 2.0f,   0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, 10.0f,   2.0f, 2.0f,   0.0f, -1.0f, 0.0f,
    };

    calcAverageNormals(indices, 12, vertices, 32, 8, 5);

    Mesh *obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(obj1);

    Mesh *obj2 = new Mesh();
    obj2->CreateMesh(floor_vertices, floor_indices, 32, 6);
    meshList.push_back(obj2);
}

void CreateShaders()
{
    Shader *shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

int main()
{
    mainWindow = Window(1280, 720);
    mainWindow.Initialise();

    createObjects();
    CreateShaders();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

    brickTexture = Texture("../OpenGL/Resources/brick.png");
    brickTexture.loadTextureA();
    groundTexture = Texture("../OpenGL/Resources/grass.png");
    groundTexture.loadTextureA();

    shinyMaterial = Material(4.0f, 256);
    dullMaterial  = Material(1.0f, 4);

    bmw = Model();
    bmw.loadModel("../OpenGL/Resources/Models/BMW X5 4.obj");

    wolf = Model();
    wolf.loadModel("../OpenGL/Resources/Models/Wolf.obj");

    cat = Model();
    cat.loadModel("../OpenGL/Resources/Models/cat.obj");

    // DIRECTIONAL LIGHT
    mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
                                 0.2f, 0.4f,
                                 0.0f, -1.0f, -1.0f);

    // POINT LIGHTS
    unsigned int pointLightCount = 0;
    pointLights[0] = PointLight(0.0, 0.0f, 1.0f,
                                0.6f, 0.1f,
                                -2.0f, 5.0f, 0.0f,
                                0.3f, 0.2f, 0.1f);
    pointLightCount++;

    pointLights[1] = PointLight(1.0f, 0.0f, 0.0f,
                                0.6f, 0.1f,
                                2.0f, 5.0f, 0.0f,
                                0.3f, 0.1f, 0.1f);
    pointLightCount++;

    // SPOT LIGHTS
    unsigned int spotLightCount = 0;
    spotLights[0] = SpotLight(1.0, 1.0f, 0.0f,
                             0.1f, 2.0f,
                             1.0f, 1.0f, 1.0f,
                             0.0f, 0.0f, -1.0f,
                             1.0f, 0.0f, 0.0f,
                             20.0f);
    spotLightCount++;

    GLfloat near = 0.01f;
    GLfloat far = 100.0f;
    glm::mat4 proj(1.0f);
    proj = glm::perspective(45.0f,
                            mainWindow.getBufferWidth()/mainWindow.getBufferHeight(),
                            near,
                            far);

    GLuint uniformProj = 0;
    GLuint uniformModel = 0;
    GLuint uniformView = 0;
    GLuint uniformEyePosition = 0;
    GLuint uniformSpecularIntensity = 0;
    GLuint uniformShininess = 0;

    float x = 0.0f;

    while(!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        x += deltaTime / 10.0f;

        mainLight = DirectionalLight(glm::max(cos(x), 0.0f), glm::max(sin(x), 0.0f), 1.0f,
                                     0.2f, 0.4f,
                                     0.0f, sin(x), -1.0f);

        glfwPollEvents();
        camera.keyControl(mainWindow.getKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        glClearColor(0.2275f/5, 0.251f/5, 0.3530f/5, 1.0f);
        //glClearColor(0.0, 0.0, 0.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProj = shaderList[0].GetProjectionLocation();
        uniformView = shaderList[0].GetViewLocation();
        uniformEyePosition = shaderList[0].GetEyePositionLocation();
        uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
        uniformShininess = shaderList[0].GetShininessLocation();

        glm::vec3 lowerLight = camera.getCameraPosition();
        lowerLight.y -= 0.1f;

        spotLights[0].setFlash(lowerLight, camera.getCameraDirection());

        shaderList[0].setDirectionalLight(&mainLight);
        shaderList[0].setPointLights(pointLights, pointLightCount);
        shaderList[0].setSpotLights(spotLights, spotLightCount);

        glUniformMatrix4fv(uniformProj, 1, GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        glUniform3f(uniformEyePosition,
                    camera.getCameraPosition().x,
                    camera.getCameraPosition().y,
                    camera.getCameraPosition().z);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
        model = glm::scale(model, glm::vec3(1.0f));
        model = glm::rotate(model, x, glm::vec3(1.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        brickTexture.useTexture();
        shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[0]->RenderMesh();


        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        groundTexture.useTexture();
        dullMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);

        meshList[1]->RenderMesh();

        // BMW
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(4.0f, -2.0f, -4.0f));
        model= glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
        bmw.renderModel();

        // WOLF
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, 4.0f));
        //model = glm::rotate(model, glm::radians(x), glm::vec3(0));
        model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
        wolf.renderModel();

        // CAT
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.0f, -1.7f, 0.0f));
        model = glm::rotate(model, x*10, glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
        cat.renderModel();

        glUseProgram(0);

        mainWindow.swapBuffers();
    }

    return 0;
}
