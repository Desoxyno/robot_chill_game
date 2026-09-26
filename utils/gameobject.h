#pragma once

#include <raylib.h>
#include "vectors.h"

class GameObject {

public:

    customMath::Vector3 position = {0, 0, 0};
    float size = 1;

    Model* model;

    GameObject(Model* model, customMath::Vector3 position)
        : model(model), position(position) {}

    virtual void Draw() {
        DrawModel(*model, raylib_vec(position), size, WHITE);
    }

    virtual void Update() {}

    virtual ~GameObject() = default;
};
