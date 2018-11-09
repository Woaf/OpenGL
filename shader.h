#ifndef SHADER_H
#define SHADER_H

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include <commonvalues.h>

#include <GL/glew.h>
#include <directionallight.h>
#include <pointlight.h>

class Shader
{
public:
    Shader();

    void CreateFromString(const char* vertexCode, const char* fragmentCode);
    void CreateFromFiles(const char* vertexLocation, const char* fragLocation);
    std::string ReadFile(const char* fileLocation);

    GLuint GetProjectionLocation()        { return uniformProj; }
    GLuint GetModelLocation()             { return uniformModel; }
    GLuint GetViewLocation()              { return uniformView; }
    GLuint GetEyePositionLocation()       { return uniformEyePosition; }
    GLuint GetAmbientIntensityLocation()  { return uniformDirectionalLight.uniformAmbientIntensity; }
    GLuint GetAmbientColorLocation()      { return uniformDirectionalLight.uniformColor; }
    GLuint GetDiffuseIntensityLocation()  { return uniformDirectionalLight.uniformDiffuseIntensity; }
    GLuint GetDirectionLocation()         { return uniformDirectionalLight.uniformDirection; }
    GLuint GetSpecularIntensityLocation() { return uniformSpecularIntensity; }
    GLuint GetShininessLocation()         { return uniformShininess; }

    void setDirectionalLight(DirectionalLight* dLight);
    void setPointLights(PointLight* pLight, unsigned int lightCount);

    void UseShader()                      { glUseProgram(shaderID); }
    void ClearShader();

    ~Shader()                             { ClearShader(); }

private:
    int pointLightCount;

    GLuint shaderID, uniformProj, uniformModel, uniformView, uniformEyePosition,
        uniformSpecularIntensity, uniformShininess;

    struct {
        GLuint uniformColor;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;

        GLuint uniformDirection;
    } uniformDirectionalLight;

    struct {
        GLuint uniformColor;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;

        GLuint uniformPosition;
        GLuint uniformConstant;
        GLuint uniformLinear;
        GLuint uniformExponent;
    } uniformPointLight[MAX_POINT_LIGHTS];

    GLuint uniformPointLightCount;

    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

#endif // SHADER_H
