#pragma once

#include "camera.h"
#include "gameobject.h"
#include <raylib.h>

class Player : public GameObject {
    public:
        PlayerCam camera;

        Player(Model &model, customMath::Vector3 position) : GameObject(&model, position), camera(PlayerCam()) {}

        void UpdCam() {
            camera.Update(position);
            UpdateCamera(&camera.camera, CAMERA_PERSPECTIVE);
        }

        void Update() override {
            UpdCam();
        }

        
};