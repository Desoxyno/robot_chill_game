#pragma once

#include "player/camera.h"
#include "utils/constant.h"
#include "core/models.h"
#include <raylib.h>

class Player : public GameObject {
    public:
        PlayerCam camera;

        Player(Model &model, customMath::Vector3 position, float rotation_y) : GameObject(&model, position, rotation_y), camera(PlayerCam()) {}

        float movement_speed = 1;

        void RegisterKey() {

            movement_speed = 10 * GetFrameTime();

            if (IsKeyDown(KEY_W)) {position.x += -cos(camera.yaw / radian) * movement_speed; position.z += -sin(camera.yaw / radian) * movement_speed;}
            if (IsKeyDown(KEY_S)) {position.x -= -cos(camera.yaw / radian) * movement_speed; position.z -= -sin(camera.yaw / radian) * movement_speed;}
            
            if (IsKeyDown(KEY_A)) {position.x -= sin(camera.yaw / radian) * movement_speed; position.z -= -cos(camera.yaw / radian) * movement_speed;}
            if (IsKeyDown(KEY_D)) {position.x += sin(camera.yaw / radian) * movement_speed; position.z += -cos(camera.yaw / radian) * movement_speed;}

        
        }

        void UpdCam() {
            camera.Update(position);
            UpdateCamera(&camera.camera, CAMERA_CUSTOM);
        }

        void Update() override {
            RegisterKey();
            UpdCam();
        }

        
};