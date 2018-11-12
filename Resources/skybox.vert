#version 330

layout (location = 0) in vec3 pos;

out vec3 texCoords;

uniform mat4 proj;
uniform mat4 view;

void main()
{
    texCoords = pos;
    gl_Position = proj * view * vec4(pos, 1.0f);
}
