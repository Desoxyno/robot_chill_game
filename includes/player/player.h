#pragma once

#include "math/vectors.h"
#include "player/camera.h"
#include "utils/constant.h"
#include "core/models.h"
#include "world/terrain.h"
#include <raylib.h>
#include <raymath.h>

class Player : public GameObject {
    public:
        PlayerCam camera;

        Model player_model = LoadModel("assets/player/bean.glb");

        Player(TerrainGeneration &terrain_gen) : GameObject(nullptr, {2500, 50, 2500}, 0), camera(PlayerCam()), terrain_gen(terrain_gen) {
            model = &player_model;
        }

        float movement_speed = 1;
        float floor_offset = 0.15;

        TerrainGeneration &terrain_gen;

        void RegisterKey() {

            movement_speed = 30 * GetFrameTime();

            if (IsKeyDown(KEY_W)) {position.x += -cos(camera.yaw / radian) * movement_speed; position.z += -sin(camera.yaw / radian) * movement_speed;}
            if (IsKeyDown(KEY_S)) {position.x -= -cos(camera.yaw / radian) * movement_speed; position.z -= -sin(camera.yaw / radian) * movement_speed;}
            
            if (IsKeyDown(KEY_A)) {position.x -= sin(camera.yaw / radian) * movement_speed; position.z -= -cos(camera.yaw / radian) * movement_speed;}
            if (IsKeyDown(KEY_D)) {position.x += sin(camera.yaw / radian) * movement_speed; position.z += -cos(camera.yaw / radian) * movement_speed;}

        }

        void checkGravity() {
            position.y = terrain_gen.getHeight(position.x, position.z) + floor_offset;
        }

        void UpdCam() {
            camera.Update(position);
            UpdateCamera(&camera.camera, CAMERA_CUSTOM);
        }

        void Update() override {
            RegisterKey();
            UpdCam();
            checkGravity();
        }

};