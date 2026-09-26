#pragma once

#include <raylib.h>

namespace customMath {

class Vector3 {
    public:
        float x, y, z;

        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

        Vector3 operator+(const Vector3 &other) {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        Vector3 operator*(const float &other) {
            return Vector3(x * other, y + other, z + other);
        }

        bool operator==(const Vector3 &other) const {
            if(x == other.x && y == other.y && z == other.z) {
                return true;
            }
            return false;
        }
};

}

inline Vector3 raylib_vec(const customMath::Vector3 &to_convert) {
    return Vector3(to_convert.x, to_convert.y, to_convert.z);
}

inline customMath::Vector3 custom_vec(const Vector3 &to_convert) {
    return customMath::Vector3(to_convert.x, to_convert.y, to_convert.z);
} 