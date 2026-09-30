#pragma once

#include <cstdint>

struct EcoVector3 {
    uint16_t x, z;
    uint8_t y;
    EcoVector3(uint16_t x, uint8_t y, uint16_t z) : x(x), y(y), z(z) {}
};

struct EcoBoundingBox {
    EcoVector3 min = {0, 0, 0};
    EcoVector3 max = {0, 0, 0};
    EcoBoundingBox() {}
};