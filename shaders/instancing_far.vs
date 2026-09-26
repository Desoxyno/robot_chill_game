#version 330

in vec3 vertexPosition;
in vec2 vertexTexCoord;

in mat4 instanceTransform;

uniform mat4 mvp;
uniform float windTime;
uniform vec3 cameraPosition;

out vec2 fragTexCoord;
out float fragDistance;

void main()
{
    vec3 localPos = vertexPosition;

    float height = clamp(vertexPosition.y, 0.0, 1.0);

    float wind = sin(
        windTime * 2.0
        + localPos.x * 0.18
        + localPos.z * 0.12
    );

    float detail = sin(
        windTime * 3.5
        + localPos.x * 0.55
        + localPos.z * 0.45
    ) * 0.25;

    float movement = wind + detail;

    localPos.x += movement * 0.12 * height * height;
    localPos.z += movement * 0.03 * height * height;

    vec4 worldPosition =
        instanceTransform * vec4(localPos, 1.0);

    fragTexCoord = vertexTexCoord;

    fragDistance =
        distance(worldPosition.xyz, cameraPosition);

    gl_Position =
        mvp * worldPosition;
}