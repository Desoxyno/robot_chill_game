#pragma once

#include "gameobject.h"
#include "vectors.h"
#include <memory>
#include <raylib.h>
#include <vector>
#include "random.h"

class Terrain {

    public:

        std::vector<std::unique_ptr<GameObject>> grass_collection;

        customMath::Vector3 position = {0, 0, 0};

        float height = 1;
        float width = 50.0f;

        Model& grass_model;

        int grass_n = 2000;

        Terrain(customMath::Vector3 position, Model& grass_model) : position(position), grass_model(grass_model) {
            GenerateTerrain();
        }

        void GenerateTerrain() {

            for (int i = 0; i < grass_n; i++) {
                
                customMath::Vector3 grass_position = {generate_rd_n(-width/2, width/2), height, generate_rd_n(-width/2, width/2)};

                grass_collection.push_back(std::make_unique<GameObject>(&grass_model, grass_position));

            }
        }

        void Draw() {

            DrawCube(raylib_vec(position), width, height, width, WHITE);

            for (auto& grass : grass_collection) {
                grass->Draw();
                // DrawCube(to_raylib_vec(grass->position), 1, 1, 1, RED);
            }
        }
};