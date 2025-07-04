#pragma once
#include "Math.h"
#include"Novice.h"

class Sphere {
public:
    Transform transform;
    float radius;
    bool isColliding = false;

    Sphere();
    void Update();
    void ShowImGuiUI();
    void Draw(const Matrix4x4& view, const Matrix4x4& proj, const Matrix4x4& vp) const;
};
