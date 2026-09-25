#pragma once

#include <memory>
#include <raylib.h>
#include <vector>

#include "vectors.h"
#include "random.h"

class GameObject {

public:

    customMath::Vector3 position = {0, 0, 0};
    float size = 1;

    Model* model;

    GameObject(Model* model, customMath::Vector3 position)
        : model(model), position(position) {}

    virtual void Draw() {
        DrawModel(*model, to_raylib_vec(position), size, WHITE);
    }

    virtual void Update() {}

    virtual ~GameObject() = default;
};


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

                customMath::Vector3 grass_position = {
                    generate_rd_n(-width, width),
                    height,
                    generate_rd_n(-width, width)
                };

                grass_collection.push_back(
                    std::make_unique<GameObject>(&grass_model, grass_position)
                );
            }
        }

        void Draw() {

            DrawCube(
                to_raylib_vec(position),
                width,
                height,
                width,
                WHITE
            );

            for (auto& grass : grass_collection) {
                grass->Draw();
            }
        }
};