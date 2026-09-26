#version 330

in vec2 fragTexCoord;
in float fragDistance;

uniform float lodStart;
uniform float lodEnd;
uniform float lodDirection;

out vec4 finalColor;

float ditherPattern(vec2 position)
{
    int x = int(mod(position.x, 4.0));
    int y = int(mod(position.y, 4.0));

    int index = x + y * 4;

    const float pattern[16] = float[16](
        0.0,  0.5,  0.125, 0.625,
        0.75, 0.25, 0.875, 0.375,
        0.1875, 0.6875, 0.0625, 0.5625,
        0.9375, 0.4375, 0.8125, 0.3125
    );

    return pattern[index];
}

void main()
{
    float transition =
        clamp(
            (fragDistance - lodStart)
            / (lodEnd - lodStart),
            0.0,
            1.0
        );

    float threshold =
        ditherPattern(gl_FragCoord.xy);

    float alpha;

    if (lodDirection > 0.0)
    {
        // Ancien LOD -> nouveau LOD
        alpha = 1.0 - transition;
    }
    else
    {
        // Nouveau LOD
        alpha = transition;
    }

    if (alpha < threshold)
        discard;

    finalColor = vec4(
        70.0 / 255.0,
        140.0 / 255.0,
        50.0 / 255.0,
        1.0
    );
}