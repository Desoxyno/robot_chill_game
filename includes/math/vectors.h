#pragma once

#include <cmath>
#include <raylib.h>
#include <raymath.h>

namespace customMath {

class Vector3 {
    public:
        float x, y, z;

        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

        Vector3 operator+(const Vector3 &other) {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        Vector3 operator+=(const Vector3 &other) {
            return Vector3(x += other.x, y += other.y, z += other.z);
        }

        Vector3 operator-(const Vector3 &other) {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }

        Vector3 operator*(const float &other) {
            return Vector3(x * other, y * other, z * other);
        }

        Vector3 operator/(const float &other) {
            return Vector3(x / other, y / other, z / other);
        }

        bool operator==(const Vector3 &other) const {
            if(x == other.x && y == other.y && z == other.z) {
                return true;
            }
            return false;
        }

};

struct HorizontalVec2 {
    float x = 0;
    float z = 0;
    HorizontalVec2(float x, float z) : x(x), z(z) {}
};

inline const float Vector3Distance(const Vector3 curr, const Vector3 targ) {
    return sqrt(pow((targ.x - curr.x), 2) + pow((targ.y - curr.y), 2) + pow((targ.z - curr.z), 2));
}

inline const float magnitude(const Vector3 &vector) {
    return sqrt(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2));
}



class Vector4 {
    public:
        float x, y, z, w;

        Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        Vector4 operator+(const Vector4 &other) {
            return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
        }

        Vector4 operator-(const Vector4 &other) {
            return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
        }

        Vector4 operator*(const double &other) {
            return Vector4(x * other, y * other, z * other, w * other);
        }

        Vector4 operator=(const double &other) {
            return Vector4(x = other, y = other, z = other, w = other);
        }

        bool operator==(const Vector4 &other) const {
            if(x == other.x && y == other.y && z == other.z) {
                return true;
            }
            return false;
        }

};

}

inline Vector3 raylib_vec(const customMath::Vector3 &to_convert) {
    return {float(to_convert.x), float(to_convert.y), float(to_convert.z)};
}

inline customMath::Vector3 custom_vec(const Vector3 &to_convert) {
    return customMath::Vector3(to_convert.x, to_convert.y, to_convert.z);
} 