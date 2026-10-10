#pragma once

#include "models.h"
#include "player/player.h"
#include "world/terrain.h"

#include <memory>
#include <raylib.h>
#include <string>
#include <utility>
#include <vector>

class Scene {

    private:

        std::vector<std::unique_ptr<GameObject>> scene_objects;
        float cpu_frame_time = 0.0f;
        
    public:
        std::unique_ptr<Terrain> terrain;

        std::unique_ptr<Player> player;

        Scene(std::unique_ptr<Player> player, std::unique_ptr<Terrain> terrain) : player(std::move(player)), terrain(std::move(terrain)) {}

        void Tick(Model &skybox) {
            float f_start = GetTime();
            Update();
            Draw(skybox, cpu_frame_time);
            cpu_frame_time = GetTime() - f_start;
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

        void Draw(Model &skybox, float cpu_frame_time) {
            BeginDrawing();

            BeginMode3D(player->camera.camera);

            rlDisableBackfaceCulling();
            rlDisableDepthMask();

            DrawModel(skybox, player->camera.camera.position, 1.0, WHITE);

            rlEnableDepthMask();
            rlEnableBackfaceCulling();

            player->Draw();
            terrain->Draw(player->camera.camera);

            for (auto& object : scene_objects) {
                object->Draw();
            }

            EndMode3D();

            DrawText(("Chunks visibles:" + std::to_string(terrain->chunks_visible) + "/" + std::to_string(terrain->terrain_gen.chunks.size())).c_str(), 10, 30, 25, WHITE);
            DrawText(("LOD0:" + std::to_string(terrain->chunks_draw_LOD0)).c_str(), 10, 55, 25, WHITE);
            DrawText(("LOD1:" + std::to_string(terrain->chunks_draw_LOD1)).c_str(), 10, 80, 25, WHITE);
            DrawText(("LOD2:" + std::to_string(terrain->chunks_draw_LOD2)).c_str(), 10, 105, 25, WHITE);
            DrawText(("LOD3:" + std::to_string(terrain->chunks_draw_LOD3)).c_str(), 10, 130, 25, WHITE);
            DrawText(("CPU frame time: " + std::to_string(cpu_frame_time * 1000) + "ms").c_str(), 10, 155, 25, WHITE);
            // DrawText(("GPU frame time: " + std::to_string(gpu_frame_time * 1000) + "ms").c_str(), 10, 180, 25, WHITE);

            DrawFPS(10, 10);

            EndDrawing();
                
        }
};