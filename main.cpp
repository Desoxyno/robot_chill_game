#include "core/scene_manager.h"
#include "raylib.h"
#include "player/player.h"
#include "core/scene.h"
#include <memory>
#include "world/terrain.h"
#include "entities/robot.h"
#include "utils/config.h"
#include "music/music_player.h"

int main() {

    SetConfigFlags( FLAG_MSAA_4X_HINT);
    InitWindow(1280, 720, "Game");
    InitAudioDevice();
    
    Config conf = Config("config/config.cfg");

    Model player_model = (LoadModel(conf.getRESpath("model_player").c_str()));
    Texture2D sky = LoadTexture(conf.getRESpath("texture_stars").c_str());
    Shader cloud_shader = LoadShader(conf.getRESpath("shader_skybox_vs").c_str(), conf.getRESpath("shader_skybox_fs").c_str());

    std::unique_ptr<Terrain> test_terrain = std::make_unique<Terrain>(customMath::Vector3{0, 0, 0});

    std::unique_ptr<Player> player = std::make_unique<Player>(test_terrain->terrain_gen);
    
    Robot robot = Robot(test_terrain->terrain_gen);

    Mesh sphere = GenMeshSphere(100.0f, 100.0f, 100.0f);
    Model skybox = LoadModelFromMesh(sphere);

    skybox.materials[0].shader = cloud_shader;
    skybox.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = sky;

    std::unique_ptr<Scene> base_scene = std::make_unique<Scene>(std::move(player), std::move(test_terrain), skybox);
    base_scene->AddObject(std::make_unique<Robot>(robot));

    SceneManager scene_manager;
    scene_manager.AddSceneToQueue(std::move(base_scene));

    DisableCursor();

    while (!WindowShouldClose()) {

        ClearBackground(WHITE);

        UpdateMusicStream(current_music);
        scene_manager.Tick();

    }

    CloseWindow();

    return 0;
}