#pragma once

#include "models.h"
#include "player/player.h"
#include "world/terrain.h"

#include <memory>
#include <raylib.h>
#include <utility>
#include <vector>

#ifdef DEBUG_BUILD
    #include <iostream>
    #include <chrono>
    #include "utils/performance_mesurer.h"
#endif

class Scene {

    private:

        std::vector<std::unique_ptr<GameObject>> scene_objects;
        float cpu_frame_time = 0.0f;
        
    public:
        std::unique_ptr<Terrain> terrain;

        std::unique_ptr<Player> player;

        Scene(std::unique_ptr<Player> player, std::unique_ptr<Terrain> terrain) : player(std::move(player)), terrain(std::move(terrain)) {}

        void Tick(Model &skybox) {
            #ifdef DEBUG_BUILD
                const auto start = std::chrono::steady_clock::now();
            #endif

            Update();
            Draw(skybox);

            #ifdef DEBUG_BUILD
                const auto elapsed = std::chrono::steady_clock::now() - start;
                cpu_frame_time = std::chrono::duration<float>(elapsed).count();
            #endif

        }

        void Update() {

            player->Update();
            terrain->Update(player->camera.camera);

            for (auto& object : scene_objects) {
                object->Update();
            }
                
        }

        void AddObject(std::unique_ptr<GameObject> to_add) {
            scene_objects.push_back(std::move(to_add));
        }

        void Draw(Model &skybox) {
            BeginDrawing();

            BeginMode3D(player->camera.camera);

            rlDisableBackfaceCulling();
            rlDisableDepthMask();

            DrawModel(skybox, player->camera.camera.position, 1.0, WHITE);

            rlEnableDepthMask();
            rlEnableBackfaceCulling();

            player->Draw();
            terrain->Draw(player->camera.camera);

            for (auto &object : scene_objects) {
                object->Draw();
            }

            EndMode3D();

            DrawFPS(10, 10);

            #ifdef DEBUG_BUILD
                std::cout << "CPU Frame Time : " << cpu_frame_time * 1000 << "ms" << std::endl;
                frame_ms_measures.push_back(cpu_frame_time);
            #endif

            EndDrawing();
                
        }

        ~Scene() {
            UnloadModel(*player->model);

            UnloadModel(terrain->renderer.grass_model_near);
            UnloadModel(terrain->renderer.grass_model_far);
            UnloadModel(terrain->renderer.grass_model_very_far);
            UnloadModel(terrain->renderer.grass_model_farthest);

            UnloadMesh(terrain->data.ground);

            for (auto &model : scene_objects) {
                UnloadModel(*model->model);
            }

        }

};