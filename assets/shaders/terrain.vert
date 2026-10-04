#version 330

in vec3 vertexPosition;
in vec2 vertexTexCoord;

uniform mat4 mvp;

uint hash32(uint value)
{
    uint x = value;

    x = ((x >> 16) ^ x) * 0x45d9f3bu;
    x = ((x >> 16) ^ x) * 0x45d9f3bu;
    x = (x >> 16) ^ x;

    return x;
}


float random01(uint hash)
{
    return float(hash32(hash)) / 4294967295.0;
}

void main()
{   
    int size = 100;

    vec3 localPosition = vertexPosition;

    float xg = floor(localPosition.x / size);
    float zg = floor(localPosition.z / size);

    uint hashInput = uint(xg) * 0x45d9f3bu + uint(zg) * 0x23e8f9cu;
    uint hash = hash32(hashInput);

    gl_Position = mvp * vec4(localPosition, 1.0);
}