#pragma once

#include <raylib.h>

namespace customMath {

class Vector3 {
    public:
        double x, y, z;

        Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

        Vector3 operator+(const Vector3 &other) {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        Vector3 operator-(const Vector3 &other) {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }

        Vector3 operator*(const double &other) {
            return Vector3(x * other, y * other, z * other);
        }

        Vector3 operator/(const double &other) {
            return Vector3(x / other, y / other, z / other);
        }

        bool operator==(const Vector3 &other) const {
            if(x == other.x && y == other.y && z == other.z) {
                return true;
            }
            return false;
        }
};

class Vector4 {
    public:
        double x, y, z, w;

        Vector4(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

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