#include "raylib.h"
#include "player/player.h"
#include "core/scene.h"
#include <memory>
#include "world/terrain.h"
#include "entities/robot.h"
#include "utils/config.h"

int main() {

    InitWindow(1280, 720, "Game");
    SetConfigFlags( FLAG_MSAA_4X_HINT);

    Config conf = Config("config/config.cfg");

    Model player_model = (LoadModel(conf.getRESpath("model_player").c_str()));
    Texture2D sky = LoadTexture(conf.getRESpath("texture_stars").c_str());
    Shader cloud_shader = LoadShader(conf.getRESpath("shader_skybox_vs").c_str(), conf.getRESpath("shader_skybox_fs").c_str());

    std::unique_ptr<Player> player = std::make_unique<Player>(Player(player_model, {2500, 0.35, 2500}, 0));
    std::unique_ptr<Terrain> test_terrain = std::make_unique<Terrain>(Terrain({0, 0, 0}));

    Scene test_map = Scene(std::move(player), std::move(test_terrain));

    Mesh sphere = GenMeshSphere(100.0f, 100.0f, 100.0f);
    Model skybox = LoadModelFromMesh(sphere);
    Robot robot = Robot(test_map.terrain->width);

    skybox.materials[0].shader = cloud_shader;
    skybox.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = sky;

    test_map.AddObject(std::make_unique<Robot>(robot));

    // DisableCursor();

    while (!WindowShouldClose()) {

        ClearBackground(WHITE);

        test_map.Update();
        test_map.Draw(skybox);

    }

    CloseWindow();

    return 0;
}