#pragma once

#include "core/models.h"
#include "math/vectors.h"
#include "utils/random.h"
#include "world/terrain.h"
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <raylib.h>
#include <raymath.h>

class Robot : public GameObject{
    public:
        customMath::HorizontalVec2 target_pos = {0, 0};

        uint8_t speed = 5;
        uint8_t turning_speed = 90.0f; // In Degrees / s

        uint8_t distance = 1;

        uint8_t wander_radius = 100;

        float angle = 0;

        float floor_offset = 0.08;

        Model robot = LoadModel("assets/robots/lil_robot.glb");

        TerrainGeneration &terrain_gen;

        Robot(TerrainGeneration &terrain_gen) : GameObject(&robot, {2500, 10, 2500}, 0), terrain_gen(terrain_gen) {}

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
            position.y = terrain_gen.getHeight(position.x, position.z) + floor_offset;
        }

        void Draw() override {
            DrawModelEx(*model, raylib_vec(position), raylib_vec({0, 1, 0}), angle, {12, 12, 12}, WHITE);
            DrawModelEx(*model, {target_pos.x, position.y, target_pos.z}, raylib_vec({0, 1, 0}), angle, {1, 1, 1}, {255, 0, 0, 120});
        }

        void Update() override {
            checkGravity();
            
            if (customMath::magnitude(customMath::Vector3{target_pos.x, position.y, target_pos.z} - position) < distance) {
                target_pos = {position.x + generate_n(-wander_radius, wander_radius), position.z + generate_n(-wander_radius, wander_radius)};
            }
            else {

                customMath::Vector3 direction = customMath::Vector3{target_pos.x, position.y, target_pos.z} - position;
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

                }

            }
        }



};