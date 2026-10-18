#pragma once

#include "core/random_events.h"
#include "music/music_player.h"
#include "scene.h"
#include <memory>
#include <utility>

class SceneManager {
    private:
        std::unique_ptr<Scene> current_scene;
        std::unique_ptr<Scene> queued_scene;
        bool can_random_event = true;

        std::unique_ptr<Scene> man_scene;

    public:
        void Tick() {
            if (queued_scene != nullptr) [[unlikely]] {
                current_scene = std::move(queued_scene);
            }

            if (can_random_event) [[likely]] {
                if (randomEvent() == 0) [[unlikely]] {
                    playMusic(0);
                    can_random_event = false;
                    AddSceneToQueue(std::move(man_scene));
                }

            }

            current_scene->Tick();
        }
        void AddSceneToQueue(std::unique_ptr<Scene> scene_to_queue) {
            queued_scene = std::move(scene_to_queue);
        }
};