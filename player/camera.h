#pragma once

#include <cmath>
#include <raylib.h>
#include "vectors.h"
#include "constant.h"

class PlayerCam {
    public:
        Camera3D camera;

        float distance = 5;
        double yaw = 0;
        double pitch = 0;

        PlayerCam() {
            camera.target = Vector3{0, 0, 0};
            camera.up = Vector3{0, 1, 0};
            camera.fovy = 45;
            camera.projection = CAMERA_PERSPECTIVE;
        }

        void Update(customMath::Vector3 target_position) {
            distance += -GetMouseWheelMove();
            if (distance > 25) {distance = 25;}

            yaw += GetMouseDelta().x / 10;
            
            pitch += GetMouseDelta().y / 10;
            if (pitch <= -60) {pitch = -60;}
            if (pitch >= 60) {pitch = 60;}

            camera.position = raylib_vec(target_position + customMath::Vector3{cos(yaw / radian) * cos(pitch / radian) * distance, sin(pitch / radian) * distance, sin(yaw / radian) * cos(pitch / radian) * distance});

            camera.target = raylib_vec(target_position + customMath::Vector3{0, 4, 0});

        }
};