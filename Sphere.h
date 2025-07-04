#pragma once
#include "Math.h"
#include"Novice.h"
#include"RenderContext.h"

class Sphere {
public:
    Transform transform;
    float radius;
    bool isColliding = false;

    Sphere();
    void Update();
    void ShowImGuiUI();
    void Draw(const RenderContext& ctx) const;
};
