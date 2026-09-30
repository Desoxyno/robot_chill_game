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
        
        
    public:
        std::unique_ptr<Terrain> terrain;

        std::unique_ptr<Player> player;

        Scene(std::unique_ptr<Player> player, std::unique_ptr<Terrain> terrain) : player(std::move(player)), terrain(std::move(terrain)) {}

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

            for (auto& object : scene_objects) {
                object->Draw();
            }

            EndMode3D();

            DrawText(("Chunks visibles:" + std::to_string(terrain->chunks_visible) + "/" + std::to_string(terrain->chunks.size())).c_str(), 10, 30, 25, WHITE);
            DrawText(("LOD0:" + std::to_string(terrain->chunks_draw_LOD0)).c_str(), 10, 55, 25, WHITE);
            DrawText(("LOD1:" + std::to_string(terrain->chunks_draw_LOD1)).c_str(), 10, 80, 25, WHITE);
            DrawText(("LOD2:" + std::to_string(terrain->chunks_draw_LOD2)).c_str(), 10, 105, 25, WHITE);
            DrawText(("LOD3:" + std::to_string(terrain->chunks_draw_LOD3)).c_str(), 10, 130, 25, WHITE);

            DrawFPS(10, 10);

            EndDrawing();
                
        }
};