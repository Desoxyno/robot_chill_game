#pragma once

#include "math/vectors.h"
#include "player/camera.h"
#include "utils/constant.h"
#include "core/models.h"
#include <raylib.h>
#include <raymath.h>

class Player : public GameObject {
    public:
        PlayerCam camera;

        Player(Model &model, customMath::Vector3 position, float rotation_y, Mesh* floor_mesh) : GameObject(&model, position, rotation_y), camera(PlayerCam()), floor_mesh(floor_mesh) {}

        float movement_speed = 1;
        float floor_offset = 0.15;

        Mesh* floor_mesh;

        void RegisterKey() {

            movement_speed = 30 * GetFrameTime();

            if (IsKeyDown(KEY_W)) {position.x += -cos(camera.yaw / radian) * movement_speed; position.z += -sin(camera.yaw / radian) * movement_speed;}
            if (IsKeyDown(KEY_S)) {position.x -= -cos(camera.yaw / radian) * movement_speed; position.z -= -sin(camera.yaw / radian) * movement_speed;}
            
            if (IsKeyDown(KEY_A)) {position.x -= sin(camera.yaw / radian) * movement_speed; position.z -= -cos(camera.yaw / radian) * movement_speed;}
            if (IsKeyDown(KEY_D)) {position.x += sin(camera.yaw / radian) * movement_speed; position.z += -cos(camera.yaw / radian) * movement_speed;}

        }

        void checkGravity() {
            Ray ray = {raylib_vec(position - customMath::Vector3{0, floor_offset, 0}), {0, -1, 0}};
            RayCollision ray_collide = GetRayCollisionMesh(ray, *floor_mesh, MatrixIdentity());
            float error = floor_offset - ray_collide.distance;
            if (!ray_collide.hit) {return;}
            else {position.y += error;}
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