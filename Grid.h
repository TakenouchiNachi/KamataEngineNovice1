#pragma once
#include "Math.h"
#include"Novice.h"

class Grid {
public:
    Transform transform;
    Grid();
    void Update();
    void Draw(const Matrix4x4& view, const Matrix4x4& proj, const Matrix4x4& vp) const;
};
