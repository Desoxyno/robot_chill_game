#include <cmath>
#include <cstdint>
#include <raylib.h>
#include <raymath.h>
#include <vector>

class RobotLeg {
    public:
        std::vector<Model> models;

        std::vector<Vector3> joints; // Position de l'articulation

        std::vector<float> lengths; // lengths[ i ] = distance of joints[ i ] to joints[ i + 1 ]

        std::vector<float> followDistance;
        // std::vector<float> angleLimits;

        Vector3 robot_pos;

        RobotLeg(std::vector<Vector3> &joints, std::vector<float> &lengths, std::vector<float> &followDistance) 
        : joints(joints), lengths(lengths), followDistance(followDistance) {
            for (int i = 0; i <  lengths.size(); i++) {
                Mesh mesh = GenMeshCube(lengths[i] , 5, lengths[i]);
                models.push_back(LoadModelFromMesh(mesh));
            }
            
        }

        ~RobotLeg() {
            for (auto &model : models) {
                UnloadModel(model);
            }
        }

        Vector3 getmemberPosition(Vector3 a, Vector3 b) {
            return (a + b) / 2;
        }

        Vector3 nextJointPosition(uint8_t index) {
            Vector3 new_pos = Vector3One();
            float distance = Vector3Distance(joints[index], joints[index + 1]);
            Vector3 direction = getNormalizedDirection(joints[index], joints[index + 1]);
            if (distance > lengths[index]) {
                new_pos = joints[index] + direction * lengths[index];
                return new_pos;
            }
            return joints[index + 1];
        }

        void Update(Vector3 robot_position) {
            robot_pos = robot_position;
            for (int i = 0; i < lengths.size(); i++) {
                joints[i + 1] = nextJointPosition(i);
            }
        }

        float getMagnitude(Vector3 &vec) {
            return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
        }

        Vector3 getNormalizedDirection(Vector3 &a, Vector3 &b) {
            Vector3 direction = b - a;
            float magnitude = getMagnitude(direction);
            Vector3 normalized = Vector3(direction.x / magnitude, direction.y / magnitude, direction.z / magnitude);
            return normalized;
        }

        void Draw() {
            for (int i = 0; i < lengths.size(); i++) {
                Vector3 position = getmemberPosition(joints[i], joints[i + 1]);
                DrawModel(models[i], position + robot_pos, 1, WHITE);
            }
        }


};
