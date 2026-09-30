#version 330

in vec3 vertexPosition;

uniform mat4 matView;
uniform mat4 matProjection;

out vec3 vDirection;

void main()
{
    vDirection = normalize(vertexPosition);
    gl_Position = matProjection * mat4(mat3(matView)) * vec4(vertexPosition, 1.0);
}