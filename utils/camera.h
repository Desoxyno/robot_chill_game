#pragma once

#include <raylib.h>
#include "vectors.h"

class PlayerCam {
    public:
        Camera3D camera;

        customMath::Vector3 offset = {-10, 10, 30};

        PlayerCam() {
            camera.target = Vector3{0, 0, 0};
            camera.up = Vector3{0, 1, 0};
            camera.fovy = 45;
            camera.projection = CAMERA_PERSPECTIVE;
        }

        void Update(customMath::Vector3 target_position) {
            camera.position = to_raylib_vec( target_position + offset);

        }
};