#version 330

in vec3 vDirection;

out vec4 fragColor;

uniform float uTime;

float hash(vec3 p)
{
    p = fract(p * 0.3183099 + vec3(0.1, 0.2, 0.3));

    p *= 17.0;

    return fract(
        p.x * p.y * p.z *
        (p.x + p.y + p.z)
    );
}

float noise(vec3 p)
{
    vec3 i = floor(p);
    vec3 f = fract(p);

    f = f * f * (3.0 - 2.0 * f);

    float n000 = hash(i);
    float n100 = hash(i + vec3(1.0, 0.0, 0.0));
    float n010 = hash(i + vec3(0.0, 1.0, 0.0));
    float n110 = hash(i + vec3(1.0, 1.0, 0.0));

    float n001 = hash(i + vec3(0.0, 0.0, 1.0));
    float n101 = hash(i + vec3(1.0, 0.0, 1.0));
    float n011 = hash(i + vec3(0.0, 1.0, 1.0));
    float n111 = hash(i + vec3(1.0, 1.0, 1.0));

    float x00 = mix(n000, n100, f.x);
    float x10 = mix(n010, n110, f.x);
    float x01 = mix(n001, n101, f.x);
    float x11 = mix(n011, n111, f.x);

    float y0 = mix(x00, x10, f.y);
    float y1 = mix(x01, x11, f.y);

    return mix(y0, y1, f.z);
}

float fbm(vec3 p)
{
    float value = 0.0;
    float amplitude = 0.5;

    for (int i = 0; i < 5; i++)
    {
        value += noise(p) * amplitude;

        p *= 2.0;
        amplitude *= 0.5;
    }

    return value;
}

float starHash(vec3 p)
{
    return fract(
        sin(
            dot(
                p,
                vec3(
                    127.1,
                    311.7,
                    74.7
                )
            )
        ) * 43758.5453123
    );
}

float stars(vec3 dir)
{
    vec3 p = normalize(dir);

    vec3 grid = floor(p * 1100.0);

    float random = starHash(grid);

    float probability = 0.985;

    float present =
        step(
            probability,
            random
        );

    vec3 offset = vec3(
        starHash(
            grid +
            vec3(
                17.3,
                71.7,
                41.1
            )
        ),
        starHash(
            grid +
            vec3(
                53.1,
                11.9,
                83.7
            )
        ),
        starHash(
            grid +
            vec3(
                91.4,
                37.2,
                19.8
            )
        )
    );

    vec3 cellPosition =
        offset * 0.8 +
        0.1;

    vec3 local =
        fract(p * 1100.0);

    float distanceToStar =
        length(
            local -
            cellPosition
        );

    float size =
        0.035 +
        starHash(
            grid +
            vec3(
                43.2,
                87.1,
                23.4
            )
        ) * 0.025;

    float intensity =
        1.0 -
        smoothstep(
            0.0,
            size,
            distanceToStar
        );

    float brightness =
        0.35 +
        starHash(
            grid +
            vec3(
                12.7,
                61.4,
                97.2
            )
        ) * 1.65;

    return
        present *
        intensity *
        brightness;
}

vec3 starColor(vec3 dir)
{
    float random =
        starHash(
            floor(dir * 400.0)
        );

    vec3 blue =
        vec3(
            0.55,
            0.70,
            1.0
        );

    vec3 white =
        vec3(
            1.0,
            0.96,
            0.88
        );

    return mix(
        blue,
        white,
        random
    );
}

float sphereMask(
    vec3 dir,
    vec3 planetDir,
    float radius
)
{
    float d =
        dot(
            dir,
            normalize(planetDir)
        );

    return smoothstep(
        cos(radius),
        cos(radius * 0.82),
        d
    );
}

vec3 planet(
    vec3 dir,
    vec3 planetDir,
    float radius,
    vec3 baseColor,
    vec3 lightDir
)
{
    vec3 pDir =
        normalize(planetDir);

    float d =
        dot(
            dir,
            pDir
        );

    float edge =
        cos(radius);

    if (d < edge)
        return vec3(0.0);

    float angle =
        acos(
            clamp(
                d,
                -1.0,
                1.0
            )
        );

    vec3 tangent =
        normalize(
            dir -
            pDir * d
        );

    vec3 surfaceDir =
        normalize(
            pDir * d +
            tangent * sin(angle)
        );

    float n =
        fbm(
            surfaceDir * 9.0
        );

    vec3 darkColor =
        baseColor * 0.4;

    vec3 lightColor =
        baseColor * 1.3;

    vec3 surfaceColor =
        mix(
            darkColor,
            lightColor,
            smoothstep(
                0.2,
                0.8,
                n
            )
        );

    float light =
        max(
            dot(
                surfaceDir,
                normalize(lightDir)
            ),
            0.0
        );

    light =
        0.15 +
        light * 0.85;

    float atmosphere =
        pow(
            max(
                0.0,
                1.0 - d
            ),
            3.0
        );

    vec3 atmosphereColor =
        baseColor *
        atmosphere *
        0.5;

    return
        surfaceColor * light +
        atmosphereColor;
}

float ring(
    vec3 dir,
    vec3 planetDir,
    float innerRadius,
    float outerRadius
)
{
    vec3 pDir =
        normalize(planetDir);

    vec3 up =
        abs(pDir.y) > 0.9
        ? vec3(1.0, 0.0, 0.0)
        : vec3(0.0, 1.0, 0.0);

    vec3 right =
        normalize(
            cross(
                up,
                pDir
            )
        );

    vec3 forward =
        normalize(
            cross(
                pDir,
                right
            )
        );

    float planeDistance =
        dot(
            dir,
            pDir
        );

    if (abs(planeDistance) > 0.035)
        return 0.0;

    vec3 projected =
        dir -
        pDir * planeDistance;

    float x =
        dot(
            projected,
            right
        );

    float y =
        dot(
            projected,
            forward
        );

    float distanceFromCenter =
        length(
            vec2(x, y)
        );

    float mask =
        smoothstep(
            innerRadius,
            innerRadius + 0.012,
            distanceFromCenter
        );

    mask *=
        1.0 -
        smoothstep(
            outerRadius - 0.012,
            outerRadius,
            distanceFromCenter
        );

    float detail =
        noise(
            vec3(
                x * 100.0,
                y * 100.0,
                0.0
            )
        );

    return mask *
           (0.45 + detail * 0.55);
}

void main()
{
    vec3 dir =
        normalize(vDirection);

    vec3 bottom =
        vec3(
            0.002,
            0.003,
            0.012
        );

    vec3 top =
        vec3(
            0.008,
            0.012,
            0.035
        );

    float vertical =
        dir.y * 0.5 + 0.5;

    vec3 col =
        mix(
            bottom,
            top,
            vertical
        );

    vec3 nebulaPosition =
        dir * 3.2 +
        vec3(
            uTime * 0.003,
            uTime * 0.001,
            -uTime * 0.002
        );

    float nebula =
        fbm(
            nebulaPosition
        );

    nebula =
        smoothstep(
            0.43,
            0.74,
            nebula
        );

    vec3 purple =
        vec3(
            0.16,
            0.015,
            0.25
        );

    vec3 blue =
        vec3(
            0.015,
            0.055,
            0.22
        );

    vec3 cyan =
        vec3(
            0.01,
            0.13,
            0.18
        );

    float nebulaNoise =
        noise(
            dir * 2.2
        );

    vec3 nebulaColor =
        mix(
            purple,
            blue,
            nebulaNoise
        );

    nebulaColor =
        mix(
            nebulaColor,
            cyan,
            smoothstep(
                0.65,
                1.0,
                nebulaNoise
            )
        );

    col +=
        nebulaColor *
        nebula *
        0.75;

    float starBrightness =
        stars(dir);

    col +=
        starColor(dir) *
        starBrightness;

    vec3 brightStarDirection =
        normalize(
            vec3(
                0.2,
                0.55,
                -0.7
            )
        );

    float brightStar =
        max(
            dot(
                dir,
                brightStarDirection
            ),
            0.0
        );

    float glow =
        pow(
            brightStar,
            90.0
        );

    col +=
        vec3(
            0.75,
            0.85,
            1.0
        ) *
        glow *
        0.8;

    vec3 planet1Direction =
        normalize(
            vec3(
                0.72,
                0.18,
                -0.68
            )
        );

    float planet1 =
        sphereMask(
            dir,
            planet1Direction,
            0.12
        );

    vec3 planet1Color =
        planet(
            dir,
            planet1Direction,
            0.12,
            vec3(
                0.25,
                0.08,
                0.45
            ),
            vec3(
                -0.5,
                0.4,
                -0.8
            )
        );

    col =
        mix(
            col,
            planet1Color,
            planet1
        );

    float planetRings =
        ring(
            dir,
            planet1Direction,
            0.14,
            0.22
        );

    col +=
        vec3(
            0.55,
            0.35,
            0.22
        ) *
        planetRings *
        0.8;

    vec3 planet2Direction =
        normalize(
            vec3(
                -0.68,
                0.32,
                -0.58
            )
        );

    float planet2 =
        sphereMask(
            dir,
            planet2Direction,
            0.075
        );

    vec3 planet2Color =
        planet(
            dir,
            planet2Direction,
            0.075,
            vec3(
                0.08,
                0.32,
                0.48
            ),
            vec3(
                0.7,
                0.3,
                -0.6
            )
        );

    col =
        mix(
            col,
            planet2Color,
            planet2
        );

    vec3 planet3Direction =
        normalize(
            vec3(
                0.05,
                -0.42,
                -0.91
            )
        );

    float planet3 =
        sphereMask(
            dir,
            planet3Direction,
            0.045
        );

    vec3 planet3Color =
        planet(
            dir,
            planet3Direction,
            0.045,
            vec3(
                0.42,
                0.19,
                0.06
            ),
            vec3(
                -0.4,
                0.6,
                -0.7
            )
        );

    col =
        mix(
            col,
            planet3Color,
            planet3
        );

    vec3 moonDirection =
        normalize(
            planet1Direction +
            vec3(
                0.09,
                0.03,
                0.02
            )
        );

    float moon =
        sphereMask(
            dir,
            moonDirection,
            0.025
        );

    vec3 moonColor =
        planet(
            dir,
            moonDirection,
            0.025,
            vec3(
                0.32,
                0.34,
                0.38
            ),
            vec3(
                -0.5,
                0.4,
                -0.8
            )
        );

    col =
        mix(
            col,
            moonColor,
            moon
        );

    float horizon =
        1.0 -
        abs(dir.y);

    col *=
        0.82 +
        horizon * 0.18;

    col =
        pow(
            max(
                col,
                0.0
            ),
            vec3(0.92)
        );

    fragColor =
        vec4(
            col,
            1.0
        );
}