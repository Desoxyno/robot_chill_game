#pragma once

#include <raylib.h>
#include "vectors.h"

class GameObject {

public:

    customMath::Vector3 position = {0, 0, 0};
    float size = 1;

    Model* model;

    float rotation_y = 0;

    GameObject(Model* model, customMath::Vector3 position, float rotation_y)
        : model(model), position(position), rotation_y(rotation_y) {}

    virtual void Draw() {
        DrawModelEx(*model, raylib_vec(position), raylib_vec({0, 1, 0}), rotation_y, {1, 1, 1}, WHITE);
    }

    virtual void Update() {}

    virtual ~GameObject() = default;
};
