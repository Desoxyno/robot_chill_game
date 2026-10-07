#pragma once

#include <cstdint>

struct EcoVector3 {
    uint16_t x, z;
    uint8_t y;
    EcoVector3(uint16_t x, uint8_t y, uint16_t z) : x(x), y(y), z(z) {}
};

class EcoVector4 {
    public:
        float x, y, z, w;

        EcoVector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        EcoVector4 operator-(const EcoVector4& other) {
            return EcoVector4(x - other.x, y - other.y, z - other.z, w - other.w);
        }
        EcoVector4 operator+(const EcoVector4& other) {
            return EcoVector4(x + other.x, y + other.y, z + other.z, w + other.w);
        }
};

struct EcoBoundingBox {
    EcoVector3 min = {0, 0, 0};
    EcoVector3 max = {0, 0, 0};
    EcoBoundingBox() {}
};