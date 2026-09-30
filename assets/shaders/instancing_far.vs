#version 330

in vec3 vertexPosition;
in vec2 vertexTexCoord;

uniform mat4 mvp;
uniform uint chunkSeed;
uniform uint minx;
uniform uint maxx;
uniform uint minz;
uniform uint maxz;

uniform uint instanceStride;

flat out float variation;
out float height;

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
    uint to_hash = (index + channel + hash);
    uint hashed = hash32(to_hash);

    return float(hashed) / 4294967295.0;
}

void main()
{
    uint hash = hash32(chunkSeed);
    uint instanceIndex = uint(gl_InstanceID) * instanceStride;

    variation = 0.8 + random01(instanceIndex, 5u, hash);
    
    float x = minx + random01(instanceIndex, 0u, hash) * (maxx - minx);
    float z = minz + random01(instanceIndex, 1u, hash) * (maxz - minz);

    float rotationY = random01(instanceIndex, 2u, hash) * 6.28318530718;

    float scaleX = 1.0 + random01(instanceIndex, 3u, hash);
    float scaleY = 1.0 + random01(instanceIndex, 4u, hash);
    float scaleZ = 1.0 + random01(instanceIndex, 5u, hash);

    vec3 localPosition = vertexPosition;

    localPosition.x *= scaleX;
    localPosition.y *= scaleY;
    localPosition.z *= scaleZ;

    float c = cos(rotationY);
    float s = sin(rotationY);

    float rotatedX = localPosition.x * c - localPosition.z * s;
    float rotatedZ = localPosition.x * s + localPosition.z * c;

    localPosition.x = rotatedX;
    localPosition.z = rotatedZ;

    height = clamp(vertexPosition.y, 0.0, 1.0);

    localPosition.x += 0.12 * height * height;
    localPosition.z += 0.03 * height * height;

    vec3 worldPosition = localPosition;
    worldPosition.x += x;
    worldPosition.z += z;

    gl_Position = mvp * vec4(worldPosition, 1.0);
}