#pragma once
#include "Math.h"
#include"Novice.h"

class Grid {
public:
    Transform transform;
    Grid();
    void Update();
    Vector3 planePoint = { 0, 0, 0 };
    Vector3 planeNormal = { 0.5f, 1.0f, 0.0f };
    void Draw(const Matrix4x4& view, const Matrix4x4& proj, const Matrix4x4& vp) const;
};
