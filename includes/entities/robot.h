#pragma once

#include "core/models.h"
#include "math/vectors.h"
#include "utils/random.h"
#include <cmath>
#include <cstdlib>
#include <raylib.h>
#include <raymath.h>

class Robot : public GameObject{
    public:
        customMath::Vector3 target_pos = {0, 0, 25};

        float speed = 5;
        float turning_speed = 90; // In Degrees / s

        float distance = 1;

        float terrain_width;

        float wander_radius = 100;

        float angle = 0;

        float floor_offset = 0.5;

        Mesh* floor_mesh;

        Model robot = LoadModel("assets/robots/lil_robot.glb");

        Robot(float terrain_width, Mesh* floor_mesh) : GameObject(&robot, {2500, 0, 2500}, 0), terrain_width(terrain_width), floor_mesh(floor_mesh) {}

        void UpdateAngle(float dt, float target_angle) {
                float delta = target_angle - angle;
                if (delta > 180) {delta = delta - 360;}
                if (delta < -180) {delta = delta + 360;}

                float turning_capacity = turning_speed * dt;
                if (abs(delta) < turning_capacity) {angle = target_angle;}
                else if (delta > 0){angle += turning_capacity;}
                else if (delta < 0){angle -= turning_capacity;}
        }


        void checkGravity() {
            Ray ray = {raylib_vec(position - customMath::Vector3{0, floor_offset, 0}), {0, -1, 0}};
            RayCollision ray_collide = GetRayCollisionMesh(ray, *floor_mesh, MatrixIdentity());
            float error = floor_offset - ray_collide.distance;
            if (!ray_collide.hit) {return;}
            else {position.y += error;}
        }

        void Draw() override {
            DrawModelEx(*model, raylib_vec(position), raylib_vec({0, 1, 0}), angle, {12, 12, 12}, WHITE);
            DrawModelEx(*model, raylib_vec(target_pos + customMath::Vector3{0, 3, 0}), raylib_vec({0, 1, 0}), angle, {1, 1, 1}, {255, 0, 0, 120});
        }

        void Update() override {
            
            if (customMath::magnitude(target_pos - position) < distance) {
                target_pos = {position.x + generate_n(-wander_radius, wander_radius), 0, position.z + generate_n(-wander_radius, wander_radius)};
            }
            else {

                customMath::Vector3 direction = target_pos - position;
                float dt = GetFrameTime();
                float target_angle = (atan2(direction.x, direction.z) * RAD2DEG) - (PI/2 * RAD2DEG);

                if (angle != target_angle) {UpdateAngle(dt, target_angle);}

                else {
                    
                float magn = customMath::magnitude(direction);
        
                direction = {direction.x / magn, direction.y / magn, direction.z / magn};

                direction.x *= (speed * dt);
                direction.y *= (speed * dt);
                direction.z *= (speed * dt);

                position += direction;

                checkGravity();

                }

            }
        }



};