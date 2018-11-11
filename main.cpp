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

static GLuint uniformProj = 0,
       uniformModel = 0,
       uniformView = 0,
       uniformEyePosition = 0,
       uniformSpecularIntensity = 0,
       uniformShininess = 0,
       uniformOmniLightPos = 0,
       uniformFarPlane = 0;

static unsigned int pointLightCount = 0;
static unsigned int spotLightCount = 0;

static GLfloat rotateAngle = 0.0f;

static Window mainWindow;

static Shader directionalShadowShader;
static Shader omniShadowShader;

static Camera camera;

static Texture brickTexture;
static Texture groundTexture;

static DirectionalLight mainLight;
static PointLight pointLights[MAX_POINT_LIGHTS];
static SpotLight spotLights[MAX_SPOT_LIGHTS];

static Material shinyMaterial;
static Material dullMaterial;

static Model bmw;
static Model wolf;
static Model cat;

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

    directionalShadowShader = Shader();
    directionalShadowShader.CreateFromFiles("../OpenGL/Resources/directional_shadow_map.vert",
                                            "../OpenGL/Resources/directional_shadow_map.frag");

    omniShadowShader.CreateFromFiles("../OpenGL/Resources/omni_shadow_map.vert",
                                     "../OpenGL/Resources/omni_shadow_map.geom",
                                     "../OpenGL/Resources/directional_shadow_map.frag");
}

void renderScene()
{
    glm::mat4 model(1.0f);

    model = glm::translate(model, glm::vec3(-4.0f, 0.0f, -3.0f));
    model = glm::scale(model, glm::vec3(1.0f));
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
    model= glm::scale(model, glm::vec3(0.06, 0.06, 0.06));
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

    rotateAngle += 0.05f;

    // CAT
    model = glm::mat4(1.0f);
    model = glm::rotate(model, rotateAngle * 3.14f / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(-4.0f, -1.3f, 0.0f));
    model = glm::scale(model, glm::vec3(0.03, 0.03, 0.03));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
    cat.renderModel();
}

void directionalShadowMapPass(DirectionalLight* light)
{
    directionalShadowShader.UseShader();

    glViewport(0, 0, light->getShadowMap()->getShadowWidth(),
               light->getShadowMap()->getShadowHeight());
    light->getShadowMap()->write();
    glClear(GL_DEPTH_BUFFER_BIT);

    uniformModel = directionalShadowShader.GetModelLocation();
    glm::mat4 placeholder = light->calculateLightTransform();
    directionalShadowShader.setDirectionalLightTransform(&placeholder);

    renderScene();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight* light)
{
    omniShadowShader.UseShader();

    glViewport(0, 0, light->getShadowMap()->getShadowWidth(),
               light->getShadowMap()->getShadowHeight());
    light->getShadowMap()->write();
    glClear(GL_DEPTH_BUFFER_BIT);

    uniformModel = omniShadowShader.GetModelLocation();

    uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation();
    uniformFarPlane = omniShadowShader.GetFarPlaceLocation();

    glUniform3f(uniformOmniLightPos, light->getPosition().x, light->getPosition().y, light->getPosition().z);
    glUniform1f(uniformFarPlane, light->getFarPlane());
    omniShadowShader.setLightMatrices(light->calculateLightTransform());

    renderScene();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void renderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
    shaderList[0].UseShader();

    uniformModel = shaderList[0].GetModelLocation();
    uniformProj = shaderList[0].GetProjectionLocation();
    uniformView = shaderList[0].GetViewLocation();
    uniformEyePosition = shaderList[0].GetEyePositionLocation();
    uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
    uniformShininess = shaderList[0].GetShininessLocation();

    glViewport(0, 0, 1280, 720);

    glClearColor(0.2275f/5, 0.251f/5, 0.3530f/5, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(uniformProj, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniform3f(uniformEyePosition,
                camera.getCameraPosition().x,
                camera.getCameraPosition().y,
                camera.getCameraPosition().z);

    shaderList[0].setDirectionalLight(&mainLight);
    shaderList[0].setPointLights(pointLights, pointLightCount);
    shaderList[0].setSpotLights(spotLights, spotLightCount);
    glm::mat4 placeholder = mainLight.calculateLightTransform();
    shaderList[0].setDirectionalLightTransform(&placeholder);

    mainLight.getShadowMap()->read(GL_TEXTURE1);
    shaderList[0].setTexture(0);
    shaderList[0].setDirectionalShadowMap(1);

    glm::vec3 lowerLight = camera.getCameraPosition();
    lowerLight.y -= 0.1f;

    spotLights[0].setFlash(lowerLight, camera.getCameraDirection());

    renderScene();
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
    mainLight = DirectionalLight(2048, 2048,
                                 1.0f, 1.0f, 1.0f,
                                 0.2f, 0.4f,
                                 0.0f, -15.0f, -10.0f);

    // POINT LIGHTS
    pointLights[0] = PointLight(1024, 1024,
                                0.01f, 100.0f,
                                0.0, 0.0f, 1.0f,
                                0.6f, 0.1f,
                                -2.0f, 5.0f, 0.0f,
                                0.3f, 0.2f, 0.1f);
    pointLightCount++;

    pointLights[1] = PointLight(1024, 1024,
                                0.01f, 100.0f,
                                1.0f, 0.0f, 0.0f,
                                0.6f, 0.1f,
                                2.0f, 5.0f, 0.0f,
                                0.3f, 0.1f, 0.1f);
    pointLightCount++;

    // SPOT LIGHTS
    spotLights[0] = SpotLight(1024, 1024,
                              0.01f, 100.0f,
                              1.0, 1.0f, 1.0f,
                              0.1f, 1.0f,
                              1.0f, 1.0f, 1.0f,
                              0.0f, 0.0f, -1.0f,
                              1.0f, 0.3f, 0.1f,
                              20.0f);
    spotLightCount++;

    GLfloat near = 0.01f;
    GLfloat far = 100.0f;
    glm::mat4 proj(1.0f);
    proj = glm::perspective(glm::radians(60.0f),
                            (GLfloat)mainWindow.getBufferWidth()/(GLfloat)mainWindow.getBufferHeight(),
                            near,
                            far);



    float x = 0.0f;

    while(!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        x += deltaTime / 10.0f;

        glfwPollEvents();
        camera.keyControl(mainWindow.getKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        directionalShadowMapPass(&mainLight);
        for(size_t i = 0; i < pointLightCount; i++)
        {
            OmniShadowMapPass(&pointLights[i]);
        }
        for(size_t i = 0; i < spotLightCount; i++)
        {
            OmniShadowMapPass(&spotLights[i]);
        }
        renderPass(proj, camera.calculateViewMatrix());

        glUseProgram(0);

        mainWindow.swapBuffers();
    }

    return 0;
}
