#version 330

in vec3 fragNormal;

out vec4 finalColor;

uniform vec3 sun_direction;

void main()
{
    vec3 normal = normalize(fragNormal);
    vec3 lightDir = normalize(-sun_direction);

    float diffuse = max(dot(normal, lightDir), 0.0);

    float lighting = 0.05 + diffuse * 0.95;

    vec3 grassColor = vec3(
        70.0 / 255.0,
        140.0 / 255.0,
        50.0 / 255.0
    );

    finalColor = vec4(grassColor * lighting, 1.0);
}