#version 330

in vec3 vDirection;

uniform sampler2D texture0;

out vec4 fragColor;

const float PI = 3.14159265359;

void main()
{
    float u = atan(vDirection.x, vDirection.z);
    u += PI;
    u /= (2 * PI);

    float v = vDirection.y * 0.5 + 0.5;

    // fragColor = texture(texture0, vec2(u, v));
    fragColor = vec4(178.0 / 255.0, 1, 1, 1);
}
