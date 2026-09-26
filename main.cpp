
#include "raylib.h"
#include "player.h"
#include "scene.h"
#include <memory>
#include "terrain.h"

int main() {

    InitWindow(1280, 720, "Game");
    SetTargetFPS(60); 

    Model grass_model = (LoadModel("assets/grass.glb"));
    Model player_model = (LoadModel("assets/bean.glb"));

    std::unique_ptr<Player> player = std::make_unique<Player>(Player(player_model, {0, 2, 0}));
    std::unique_ptr<Terrain> test_terrain = std::make_unique<Terrain>(Terrain({0, 0, 0}, grass_model));

    Scene test_map = Scene(std::move(player), std::move(test_terrain));

    while (!WindowShouldClose()) {

        ClearBackground(RAYWHITE);

        test_map.Update();

        BeginDrawing();

            BeginMode3D(test_map.player->camera.camera);

                test_map.Draw();

                DrawGrid(100, 0.5f);

            EndMode3D();

        EndDrawing();


    }

    CloseWindow();

    return 0;
}