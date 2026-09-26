#version 330

in vec2 fragTexCoord;
in vec3 fragNormal;

out vec4 finalColor;

void main()
{
    vec3 lightDir =
        normalize(vec3(0.4, 1.0, 0.3));

    float lighting =
        max(
            dot(
                normalize(fragNormal),
                lightDir
            ),
            0.0
        );

    vec3 grassColor =
        vec3(
            70.0 / 255.0,
            140.0 / 255.0,
            50.0 / 255.0
        );

    grassColor *=
        0.4 + lighting * 0.6;

    finalColor =
        vec4(grassColor, 1.0);
}