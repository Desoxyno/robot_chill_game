#include "math/vectors.h"
#include <cmath>
#include <cstdint>
#include <raylib.h>
#include <raymath.h>
#include <vector>

class RobotLeg {
    public:
        std::vector<Model> models;

        std::vector<customMath::Vector3> joints; // Position de l'articulation

        std::vector<float> lengths; // lengths[ i ] = distance of joints[ i ] to joints[ i + 1 ]

        std::vector<float> followDistance;
        // std::vector<float> angleLimits;

        customMath::Vector3 robot_pos = {0, 0, 0};

        RobotLeg(std::vector<customMath::Vector3> &joints, std::vector<float> &lengths, std::vector<float> &followDistance) 
        : joints(joints), lengths(lengths), followDistance(followDistance) {
            for (int i = 0; i <  lengths.size(); i++) {
                Mesh mesh = GenMeshCube(0.15, 0.2, lengths[i]);
                models.push_back(LoadModelFromMesh(mesh));
            }
            
        }

        ~RobotLeg() {
            for (auto &model : models) {
                UnloadModel(model);
            }
        }

        customMath::Vector3 getmemberPosition(customMath::Vector3 a, customMath::Vector3 b) {
            return (a + b) / 2;
        }

        customMath::Vector3 nextJointPosition(uint8_t index) {

            customMath::Vector3 new_pos = custom_vec(Vector3One());
            customMath::Vector3 direction = getNormalizedDirection(joints[index], joints[index + 1]);

            float distance = customMath::Vector3Distance(joints[index], joints[index + 1]);

            new_pos = joints[index] + direction * lengths[index];
            return new_pos;
        
        }

        void Update(customMath::Vector3 robot_position, customMath::Vector3 anchor_point) {
            joints[0] = anchor_point;
            robot_pos = robot_position;
            for (int i = 0; i < lengths.size(); i++) {
                joints[i + 1] = nextJointPosition(i);
            }
        }

        float getMagnitude(customMath::Vector3 &vec) {
            return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
        }

        customMath::Vector3 getNormalizedDirection(customMath::Vector3 &a, customMath::Vector3 &b) {
            customMath::Vector3 direction = b - a;
            float magnitude = getMagnitude(direction);
            if (magnitude == 0) {
                return direction;
            }
            customMath::Vector3 normalized = customMath::Vector3(direction.x / magnitude, direction.y / magnitude, direction.z / magnitude);
            return normalized;
        }

        void Draw() {
            for (int i = 0; i < lengths.size(); i++) {
                customMath::Vector3 position = getmemberPosition(joints[i], joints[i + 1]);
                DrawModel(models[i], raylib_vec(position + robot_pos), 1, WHITE);
            }
            for (int i = 0; i < joints.size(); i++) {
                DrawSphere(raylib_vec(joints[i] + robot_pos), 0.1, RED);
            }
        }


};
