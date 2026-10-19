#pragma once


#include <cstdint>

struct EcoVector3 {
    uint16_t x : 13;
    uint16_t z : 13;
    uint16_t y : 4;
    EcoVector3(uint16_t x, uint8_t y, uint16_t z) : x(x), y(y), z(z) {}
};

constexpr uint32_t Y_mask = 0b00000000000000000000000000001111;
constexpr uint32_t X_mask = 0b00000000000000011111111111110000;
constexpr uint32_t Z_mask = 0b00111111111111100000000000000000;

#pragma pack ()
struct MicroVector3 {
    uint32_t position;
};

static_assert(sizeof(MicroVector3) == 4);

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



struct MicroBoundingBox {
    MicroVector3 min;
    MicroVector3 max;
    MicroBoundingBox() {}
};
