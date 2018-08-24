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
    void UseShader();
    void ClearShader();

    ~Shader();

private:
    GLuint shaderID, uniformProj, uniformModel;

    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

#endif // SHADER_H
