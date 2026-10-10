#pragma once

#include "math/vectors.h"
#include <vector>

inline std::vector<customMath::Vector3> joints = {
    {0.0f, 15.0f, 0.0f}, // J0 : attache au corps
    {0.0f, 10.0f, 0.0f}, // J1
    {0.0f, 5.0f, 0.0f}, // J2
    {0.0f, 0.0f, 0.0f}  // J3 : pied
};

inline std::vector<float> lengths = {
    5.0f, // J0 -> J1
    5.0f, // J1 -> J2
    5.0f  // J2 -> J3
};

inline std::vector<float> followDistance = {
    2.5f,
    2.5f,
    2.5f
};