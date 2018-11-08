#ifndef SHADER_H
#define SHADER_H

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader
{
public:
    Shader();

    void CreateFromString(const char* vertexCode, const char* fragmentCode);
    void CreateFromFiles(const char* vertexLocation, const char* fragLocation);
    std::string ReadFile(const char* fileLocation);

    GLuint GetProjectionLocation();
    GLuint GetModelLocation();
    GLuint GetViewLocation();
    GLuint GetEyePositionLocation();
    GLuint GetAmbientIntensityLocation();
    GLuint GetAmbientColorLocation();
    GLuint GetDiffuseIntensityLocation();
    GLuint GetDirectionLocation();
    GLuint GetSpecularIntensityLocation();
    GLuint GetShininessLocation();

    void UseShader();
    void ClearShader();

    ~Shader();

private:
    GLuint uniformProj, uniformModel, uniformView, uniformEyePosition,
    uniformAmbientIntensity, uniformAmbientColor,
    uniformDiffuseIntensity, uniformDirection,
    uniformSpecularIntensity, uniformShininess;
    GLuint shaderID;

    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

#endif // SHADER_H
