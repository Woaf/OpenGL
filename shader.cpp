#include "shader.h"

Shader::Shader()
{
    shaderID = 0;
    uniformModel = 0;
    uniformProj = 0;

    pointLightCount = 0;
    spotLightCount = 0;
}

void Shader::CreateFromString(const char *vertexCode, const char *fragmentCode)
{
    CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char *vertexLocation, const char *fragLocation)
{
    std::string vertexString = ReadFile(vertexLocation);
    std::string fragString = ReadFile(fragLocation);

    const char* vertexCode = vertexString.c_str();
    const char* fragCode = fragString.c_str();

    CompileShader(vertexCode, fragCode);

}

void Shader::CreateFromFiles(const char *vertexLocation, const char *geomLocation, const char *fragLocation)
{
    std::string vertexString = ReadFile(vertexLocation);
    std::string geomString = ReadFile(geomLocation);
    std::string fragString = ReadFile(fragLocation);

    const char* vertexCode = vertexString.c_str();
    const char* geomCode = geomString.c_str();
    const char* fragCode = fragString.c_str();

    CompileShader(vertexCode, geomCode, fragCode);
}

void Shader::validate()
{
    GLint result = 0;
    GLchar elog[1024] = {0};

    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if(!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(elog), nullptr, elog);
        printf("ERROR validating shader!%s", elog);
        return;
    }
}

std::string Shader::ReadFile(const char *fileLocation)
{
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);

    if(!fileStream.is_open())
    {
        printf("Failed to read %s! File does not exist", fileLocation);
        return "";
    }

    std::string line = "";
    while(!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line+"\n");
    }

    fileStream.close();
    return content;
}

void Shader::setDirectionalLight(DirectionalLight *dLight)
{
    dLight->useLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor,
                     uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::setPointLights(PointLight *pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset)
{
    if(lightCount > MAX_POINT_LIGHTS)
        lightCount = MAX_POINT_LIGHTS;

    glUniform1i(uniformPointLightCount, lightCount);

    for(size_t i = 0; i < lightCount; i++)
    {
        pLight[i].useLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColor,
                           uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
                           uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i]. uniformExponent);

        pLight[i].getShadowMap()->read(GL_TEXTURE0 + textureUnit + i);
        glUniform1i(uniformOmniShadowMap[i + offset].uniformShadowMap, textureUnit+i);
        glUniform1f(uniformOmniShadowMap[i + offset].uniformFarPlane, pLight[i].getFarPlane());
    }
}

void Shader::setSpotLights(SpotLight *sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset)
{
    if(lightCount > MAX_SPOT_LIGHTS)
        lightCount = MAX_SPOT_LIGHTS;

    glUniform1i(uniformSpotLightCount, lightCount);

    for(size_t i = 0; i < lightCount; i++)
    {
        sLight[i].useLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColor,
                           uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition,
                           uniformSpotLight[i].uniformDirection,
                           uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent,
                           uniformSpotLight[i].uniformEdge);
        sLight[i].getShadowMap()->read(GL_TEXTURE0 + textureUnit + i);
        glUniform1i(uniformOmniShadowMap[i + offset].uniformShadowMap, textureUnit+i);
        glUniform1f(uniformOmniShadowMap[i + offset].uniformFarPlane, sLight[i].getFarPlane());
    }
}

void Shader::setTexture(GLuint textureUnit)
{
    glUniform1i(uniformTexture, textureUnit);
}

void Shader::setDirectionalShadowMap(GLuint textureUnit)
{
    glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::setDirectionalLightTransform(glm::mat4 *lTransform)
{
    glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));
}

void Shader::setLightMatrices(std::vector<glm::mat4> lightMatrices)
{
    for(size_t i = 0; i < 6; i++)
    {
        glUniformMatrix4fv(uniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
    }
}

void Shader::ClearShader()
{
    if(shaderID != 0)
    {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }

    uniformModel = 0;
    uniformProj = 0;
}

void Shader::CompileShader(const char *vertexCode, const char *fragmentCode)
{
    shaderID = glCreateProgram();

    if(!shaderID)
    {
        printf("Error creating SHADER PROGRAM!");
        return;
    }

    AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
    AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

    compileProgram();
}

void Shader::CompileShader(const char *vertexCode, const char* geomCode, const char *fragmentCode)
{
    shaderID = glCreateProgram();

    if(!shaderID)
    {
        printf("Error creating SHADER PROGRAM!");
        return;
    }

    AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
    AddShader(shaderID, geomCode, GL_GEOMETRY_SHADER);
    AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

    compileProgram();
}

void Shader::AddShader(GLuint theProgram, const char *shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar elog[1024] = {0};

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if(!result)
    {
        glGetShaderInfoLog(theShader, sizeof(elog), nullptr, elog);
        printf("ERROR linking shader!%s", elog);
        return;
    }

    glAttachShader(theProgram, theShader);
}

void Shader::compileProgram()
{
    GLint result = 0;
    GLchar elog[1024] = {0};

    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if(!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(elog), nullptr, elog);
        printf("ERROR linking shader!%s", elog);
        return;
    }

    uniformProj = glGetUniformLocation(shaderID, "proj");
    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformView = glGetUniformLocation(shaderID, "view");
    uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderID, "dirLight.base.color");
    uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "dirLight.base.ambientIntensity");
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "dirLight.direction");
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "dirLight.base.diffuseIntensity");
    uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
    uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
    uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");

    uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

    for(size_t i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        char locBuff[100] = {'\0'};

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
        uniformPointLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
        uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
        uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
        uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
        uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
        uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
        uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
    }

    uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

    for(size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
    {
        char locBuff[100] = {'\0'};

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.color", i);
        uniformSpotLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
        uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
        uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
        uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
        uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
        uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
        uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
        uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
        uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
    }

    for(size_t i = 0; i < MAX_POINT_LIGHTS+MAX_SPOT_LIGHTS; i++)
    {
        char locBuff[100] = {'\0'};

        snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].shadowMap", i);
        uniformOmniShadowMap[i].uniformShadowMap = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].farPlane", i);
        uniformOmniShadowMap[i].uniformFarPlane = glGetUniformLocation(shaderID, locBuff);
    }

    uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform");
    uniformTexture = glGetUniformLocation(shaderID, "theTexture");
    uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");

    uniformOmniLightPos = glGetUniformLocation(shaderID, "lightPos");
    uniformFarPlane = glGetUniformLocation(shaderID, "farPlane");

    for(size_t i = 0; i < 6; i++)
    {
        char locBuff[100] = {'\0'};

        snprintf(locBuff, sizeof(locBuff), "lightMatrices[%d]", i);
        uniformLightMatrices[i] = glGetUniformLocation(shaderID, locBuff);
    }
}
