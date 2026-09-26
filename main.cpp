#include "raylib.h"
#include "player.h"
#include "scene.h"
#include <memory>
#include "terrain.h"

int main() {

    InitWindow(1280, 720, "Game");
    Model player_model = (LoadModel("assets/bean.glb"));

    std::unique_ptr<Player> player = std::make_unique<Player>(Player(player_model, {0, 2, 0}, 0));
    std::unique_ptr<Terrain> test_terrain = std::make_unique<Terrain>(Terrain({0, 0, 0}));

    Scene test_map = Scene(std::move(player), std::move(test_terrain));

    Shader cloud_shader = LoadShader("shaders/skybox.vs", "shaders/skybox.fs");
    int timeLoc = GetShaderLocation(cloud_shader, "uTime");

    Mesh cube = GenMeshCube(5000.0f, 500.0f, 5000.0f);
    Model skybox = LoadModelFromMesh(cube);
    skybox.materials[0].shader = cloud_shader;

    DisableCursor();

    while (!WindowShouldClose()) {

        ClearBackground({29, 41, 81});

        float time = GetTime();

        SetShaderValue(
            cloud_shader,
            timeLoc,
            &time,
            SHADER_UNIFORM_FLOAT
        );

        test_map.Update();
        test_map.Draw(skybox);

    }

    CloseWindow();

    return 0;
}