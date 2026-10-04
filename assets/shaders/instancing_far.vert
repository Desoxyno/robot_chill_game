#version 330

in vec3 vertexPosition;
in vec2 vertexTexCoord;

uniform mat4 mvp;
uniform uint chunkSeed;
uniform int minx;
uniform int maxx;
uniform int minz;
uniform int maxz;
uniform float windTime;

uniform uint instanceStride;

flat out float variation;
out float grassheight;

out float baseX;

uniform sampler2D terrainHeightmap;

uint hash32(uint value)
{
    uint x = value;

    x = ((x >> 16) ^ x) * 0x45d9f3bu;
    x = ((x >> 16) ^ x) * 0x45d9f3bu;
    x = (x >> 16) ^ x;

    return x;
}

float random01(uint index, uint channel, uint hash)
{
    uint to_hash = index + channel + hash;
    uint hashed = hash32(to_hash);

    return float(hashed) / 4294967295.0;
}

void main()
{
    float offset = 0;
    float spacing = float(5000) / 254.0;

    uint hash = hash32(chunkSeed);
    uint instanceIndex = uint(gl_InstanceID);

    variation = 0.8 + random01(instanceIndex, 5u, hash);

    float x = float(minx) + random01(instanceIndex, 0u, hash) * float(maxx - minx);
    float z = float(minz) + random01(instanceIndex, 1u, hash) * float(maxz - minz);

    float gridx = x / spacing;
    float gridz = z / spacing;

    float baseXValue = floor(gridx);
    float baseZ = floor(gridz);

    float fracX = fract(gridx);
    float fracZ = fract(gridz);

    baseX = baseXValue;

    int texX = int(baseZ);
    int texY = int(baseXValue);

    texX = clamp(texX, 0, 253);
    texY = clamp(texY, 0, 253);

    float A = texelFetch(terrainHeightmap, ivec2(texX, texY), 0).r;
    float B = texelFetch(terrainHeightmap, ivec2(texX + 1, texY), 0).r;
    float C = texelFetch(terrainHeightmap, ivec2(texX, texY + 1), 0).r;
    float D = texelFetch(terrainHeightmap, ivec2(texX + 1, texY + 1), 0).r;

    float height;

    if (fracX + fracZ <= 1.0)
    {
        float weightA = 1.0 - fracX - fracZ;
        float weightB = fracZ;
        float weightC = fracX;

        height = A * weightA + B * weightB + C * weightC;
    }
    else
    {
        float weightB = 1.0 - fracX;
        float weightD = fracX + fracZ - 1.0;
        float weightC = 1.0 - fracZ;

        height = B * weightB + D * weightD + C * weightC;
    }

    height -= offset;

    float rotationY = random01(instanceIndex, 2u, hash) * 6.28318530718;

    float scaleX = 1.0 + random01(instanceIndex, 3u, hash);
    float scaleY = 1.0 + random01(instanceIndex, 4u, hash);
    float scaleZ = 1.0 + random01(instanceIndex, 5u, hash);

    vec3 localPosition = vertexPosition;

    localPosition.y = height + vertexPosition.y * scaleY;

    localPosition.x *= scaleX;
    localPosition.z *= scaleZ;

    float c = cos(rotationY);
    float s = sin(rotationY);

    float rotatedX = localPosition.x * c - localPosition.z * s;
    float rotatedZ = localPosition.x * s + localPosition.z * c;

    localPosition.x = rotatedX;
    localPosition.z = rotatedZ;

    grassheight = clamp(vertexPosition.y, 0.0, 1.0);

    localPosition.x += 0.12 * grassheight * grassheight;
    localPosition.z += 0.03 * grassheight * grassheight;

    vec3 worldPosition = localPosition;

    worldPosition.x += x;
    worldPosition.z += z;

    gl_Position = mvp * vec4(worldPosition, 1.0);
}