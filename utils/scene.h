#pragma once

#include "gameobject.h"
#include "player.h"
#include "terrain.h"

#include <memory>
#include <raylib.h>
#include <utility>
#include <vector>

class Scene {

    private:

        std::vector<std::unique_ptr<GameObject>> scene_objects;
        std::unique_ptr<Terrain> terrain;
        
    public:

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

        void Draw() {
            BeginDrawing();

            BeginMode3D(player->camera.camera);

            DrawGrid(100, 0.5f);

            player->Draw();
            terrain->Draw(player->camera.camera);

            for (auto& object : scene_objects) {
                object->Draw();
            }

            EndMode3D();

            DrawFPS(10, 10);

            EndDrawing();
                
        }
};