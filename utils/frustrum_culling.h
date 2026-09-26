#pragma once

#include "constant.h"
#include "raylib.h"
#include "vectors.h"
#include <array>
#include <cmath>
#include <raymath.h>
#include "rlgl.h"

struct Frustum {
    std::array<customMath::Vector4, 6> plan_array;
    Frustum(customMath::Vector4 left, customMath::Vector4 right, customMath::Vector4 down, customMath::Vector4 up, customMath::Vector4 near, customMath::Vector4 far) : plan_array{left, right, down, up, near, far}{}
    void operator=(const Frustum &other) {
        plan_array[0] = other.plan_array[0]; plan_array[1] = other.plan_array[1]; plan_array[2] = other.plan_array[2]; plan_array[3] = other.plan_array[3]; plan_array[4] = other.plan_array[4]; plan_array[5] = other.plan_array[5];
    }

};

inline bool IsBoxOutsidePlane(const BoundingBox& box, const customMath::Vector4& plane) {
    double x = plane.x >= 0 ? box.max.x : box.min.x;
    double y = plane.y >= 0 ? box.max.y : box.min.y;
    double z = plane.z >= 0 ? box.max.z : box.min.z;

    double distance =
        plane.x * x +
        plane.y * y +
        plane.z * z +
        plane.w;

    return distance < 0;
}

inline bool IsBoxOutsideFrustum(const BoundingBox& box, const Frustum& frustum)
{
    for (const auto& plane : frustum.plan_array)
    {
        if (IsBoxOutsidePlane(box, plane))
            return true;
    }

    return false;
}

inline Frustum GetFrustum(const Camera3D &camera) {
    Matrix view = GetCameraMatrix(camera);
    Matrix projection = MatrixPerspective(camera.fovy * radian, float(GetScreenWidth()) / float(GetScreenHeight()), rlGetCullDistanceNear(), rlGetCullDistanceFar());

    Matrix viewprojection = MatrixMultiply(view, projection);

    customMath::Vector4 R1 = {viewprojection.m0, viewprojection.m4, viewprojection.m8, viewprojection.m12};
    customMath::Vector4 R2 = {viewprojection.m1, viewprojection.m5, viewprojection.m9, viewprojection.m13};
    customMath::Vector4 R3 = {viewprojection.m2, viewprojection.m6, viewprojection.m10, viewprojection.m14};
    customMath::Vector4 R4 = {viewprojection.m3, viewprojection.m7, viewprojection.m11, viewprojection.m15};

    customMath::Vector4 left = R4 + R1;
    float left_length = sqrt(left.x * left.x + left.y * left.y + left.z * left.z);
    left.x /= left_length; left.y /= left_length; left.z /= left_length; left.w /= left_length;

    customMath::Vector4 right = R4 - R1;
    float right_length = sqrt(right.x * right.x + right.y * right.y + right.z * right.z);
    right.x /= right_length; right.y /= right_length; right.z /= right_length; right.w /= right_length;

    customMath::Vector4 down = R4 + R2;
    float down_length = sqrt(down.x * down.x + down.y * down.y + down.z * down.z);
    down.x /= down_length; down.y /= down_length; down.z /= down_length; down.w /= down_length;

    customMath::Vector4 up = R4 - R2;
    float up_length = sqrt(up.x * up.x + up.y * up.y + up.z * up.z);
    up.x /= up_length; up.y /= up_length; up.z /= up_length; up.w /= up_length;

    customMath::Vector4 near = R4 + R3;
    float near_length = sqrt(near.x * near.x + near.y * near.y + near.z * near.z);
    near.x /= near_length; near.y /= near_length; near.z /= near_length; near.w /= near_length;

    customMath::Vector4 far = R4 - R3;
    float far_length = sqrt(far.x * far.x + far.y * far.y + far.z * far.z);
    far.x /= far_length; far.y /= far_length; far.z /= far_length; far.w /= far_length;

    return Frustum(left, right, down, up, near, far);
}