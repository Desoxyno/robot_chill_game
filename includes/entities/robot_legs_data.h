#pragma once

#include <raylib.h>
#include <vector>

inline std::vector<Vector3> joints = {
    {0.0f, 3.0f, 0.0f}, // J0 : attache au corps
    {0.0f, 2.0f, 0.0f}, // J1
    {0.0f, 1.0f, 0.0f}, // J2
    {0.0f, 0.0f, 0.0f}  // J3 : pied
};

inline std::vector<float> lengths = {
    1.0f, // J0 -> J1
    1.0f, // J1 -> J2
    1.0f  // J2 -> J3
};

inline std::vector<Vector3> positions = {
    {0.0f, 2.5f, 0.0f}, // membre 0
    {0.0f, 1.5f, 0.0f}, // membre 1
    {0.0f, 0.5f, 0.0f}  // membre 2
};

inline std::vector<float> followDistance = {
    0.5f,
    0.5f,
    0.5f
};