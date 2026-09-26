#version 330

in vec3 vertexPosition;

out vec3 vDirection;

uniform mat4 mvp;

void main()
{
    vDirection = vertexPosition;

    gl_Position = mvp * vec4(vertexPosition, 1.0);
}