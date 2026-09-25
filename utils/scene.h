#pragma once

#include "gameobject.h"
#include "player.h"
#include <memory>
#include <utility>
#include <vector>

class Scene {

    private:

        std::vector<std::unique_ptr<GameObject>> scene_objects;
        
    public:

        std::unique_ptr<Player> player;

        Scene(std::unique_ptr<Player> player) : player(std::move(player)) {}

        void Update() {
            for (auto& object : scene_objects)
                object->Update();
        }

        void AddObject(std::unique_ptr<GameObject> to_add) {
            scene_objects.push_back(std::move(to_add));
        }

        void Draw() {
            for (auto& object : scene_objects)
                object->Draw();
        }
};