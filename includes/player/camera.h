#pragma once

#include <cmath>
#include <raylib.h>
#include "math/vectors.h"
#include "utils/constant.h"

class PlayerCam {
    public:
        Camera3D camera;

        float distance = 5;
        double yaw = 0;
        double pitch = 0;

        PlayerCam() {
            camera.target = {0, 0, 0};
            camera.up = {0, 1, 0};
            camera.fovy = 45;
            camera.projection = CAMERA_PERSPECTIVE;
        }

        void Update(customMath::Vector3 target_position) {
            distance += -GetMouseWheelMove();
            if (distance > 25) {distance = 25;}
            if (distance < 1) {distance = 1;}

            yaw += GetMouseDelta().x / 10;
            
            pitch += GetMouseDelta().y / 10;
            if (pitch <= -60) {pitch = -60;}
            if (pitch >= 60) {pitch = 60;}

            camera.position = raylib_vec(target_position + customMath::Vector3{static_cast<float>(cos(yaw / radian) * cos(pitch / radian) * distance), static_cast<float>(sin(pitch / radian) * distance), static_cast<float>(sin(yaw / radian) * cos(pitch / radian) * distance)});

            camera.target = raylib_vec(target_position + customMath::Vector3{0, 4, 0});

        }
};