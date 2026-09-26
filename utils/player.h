#pragma once

#include "camera.h"
#include "gameobject.h"
#include <raylib.h>

class Player : public GameObject {
    public:
        PlayerCam camera;

        Player(Model &model, customMath::Vector3 position) : GameObject(&model, position), camera(PlayerCam()) {}

        int movement_speed = 1;

        void RegisterKey() {

            if (IsKeyDown(KEY_W)) {position.x += movement_speed;}
            if (IsKeyDown(KEY_S)) {position.x -= movement_speed;}
            
            if (IsKeyDown(KEY_A)) {position.z -= movement_speed;}
            if (IsKeyDown(KEY_D)) {position.z += movement_speed;}

        
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