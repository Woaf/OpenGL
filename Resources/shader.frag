#version 330

in vec4 vCol;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;
in vec4 directionalLightSpacePos;

out vec4 color;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light
{
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight
{
    Light base;
    vec3 direction;
};

struct PointLight
{
    Light base;
    vec3 position;
    float constant;
    float linear;
    float exponent;
};

struct SpotLight
{
    PointLight base;
    vec3 direction;
    float edge;
};

struct OmniShadowMap
{
    samplerCube shadowMap;
    float farPlane;
};

struct Material
{
    float specularIntensity;
    float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight dirLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

uniform sampler2D theTexture;
uniform sampler2D directionalShadowMap;

uniform Material material;

uniform vec3 eyePosition;

vec3 sampleOffsetDirections[20] = vec3[]
(
    vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
    vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
    vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
    vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
    vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
);

vec4 calcLightByDirection(Light light, vec3 direction, float shadowFactor)
{
    vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;

    float diffuseFactor = max(dot(normalize(normal), normalize(direction)), 0.0f);
    vec4 diffuseColor = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0f);

    vec4 specularColor = vec4(0, 0, 0, 0);

    if(diffuseFactor > 0.0f)
    {
        vec3 fragToEye = normalize(eyePosition - fragPos);
        vec3 reflectedVertex = normalize(reflect(direction, normalize(normal)));

        float specularFactor = dot(fragToEye, reflectedVertex);
        if(specularFactor > 0.0f)
        {
            specularFactor = pow(specularFactor, material.shininess);
            specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
        }
    }

    return (ambientColor + (1.0f - shadowFactor) * (diffuseColor + specularColor));
}

float calcPointShadowFactor(PointLight light, int shadowIndex)
{
    vec3 fragToLight = fragPos - light.position;
    float currentDepth = length(fragToLight);

    float shadow = 0.0f;
    float bias   = 0.15f;
    int samples = 20;
    float viewDistance = length(eyePosition - fragPos);
    float diskRadius = (1.0 + (viewDistance / omniShadowMaps[shadowIndex].farPlane)) / 25.0;
    for(int i = 0; i < samples; i++)
    {
        float closestDepth = texture(omniShadowMaps[shadowIndex].shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= omniShadowMaps[shadowIndex].farPlane;
        if(currentDepth - bias > closestDepth)
        {
            shadow += 1.0f;
        }
    }
    shadow /= float(samples);
    return shadow;
}

float calcShadowFactor(vec4 directionalLightSpacePos)
{
    vec3 projCoords = directionalLightSpacePos.xyz / directionalLightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(directionalShadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    vec3 Normal = normalize(normal);
    vec3 lightDir = normalize(dirLight.direction);

    float bias = max(0.05 * (1.0 - dot(Normal, lightDir)), 0.0005);

    float shadow = 0.0f;
    vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
    for(int x = -1; x <= 1; ++x)
            {
                    for(int y = -1; y <= 1; ++y)
                    {
                            float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x,y) * texelSize).r;
                            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
                    }
            }

            shadow /= 9.0;
            if(projCoords.z > 1.0)
            {
                    shadow = 0.0;
            }

            return shadow;
}

vec4 calcDirectionalLight(vec4 DirectionalLightSpacePos)
{
    float shadowFactor = calcShadowFactor(DirectionalLightSpacePos);
    return calcLightByDirection(dirLight.base, dirLight.direction, shadowFactor);
}

vec4 calcPointLight(PointLight pLight, int shadowIndex)
{
    vec3 direction = fragPos - pLight.position;
    float distance = length(direction);
    direction = normalize(direction);

    float shadowFactor = calcPointShadowFactor(pLight, shadowIndex);

    vec4 color = calcLightByDirection(pLight.base, direction, shadowFactor);
    float attenuation = pLight.exponent * distance * distance +
            pLight.linear * distance +
            pLight.constant;

    return (color / attenuation);
}

vec4 calcSpotLight(SpotLight sLight, int shadowIndex)
{
    vec3 rayDirection = normalize(fragPos - sLight.base.position);
    float slFactor = dot(rayDirection, sLight.direction);

    if(slFactor > sLight.edge)
    {
        vec4 color = calcPointLight(sLight.base, shadowIndex);

        return color * (1.0f - (1.0f - slFactor) * (1.0f / (1.0f - sLight.edge)));
    }
    else
    {
        return vec4(0, 0, 0, 0);
    }
}

vec4 calcPointLights()
{
    vec4 totalColor = vec4(0, 0, 0, 0);
    for(int i = 0; i < pointLightCount; i++)
    {
        totalColor += calcPointLight(pointLights[i], i);
    }
    return totalColor;
}

vec4 calcSpotLights()
{
    vec4 totalColor = vec4(0, 0, 0, 0);
    for(int i = 0; i < spotLightCount; i++)
    {
        totalColor += calcSpotLight(spotLights[i], i + pointLightCount);
    }
    return totalColor;
}

void main()
{
    vec4 finalColor = calcDirectionalLight(directionalLightSpacePos);
    finalColor += calcPointLights();
    finalColor += calcSpotLights();
    color = texture(theTexture, texCoord) * finalColor;
}
