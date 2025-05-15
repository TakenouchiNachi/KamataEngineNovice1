#pragma once
#include "Math.h"
#include"Novice.h"

class Sphere {
public:
    Transform transform;
    Sphere();
    void Update();
    void Draw(const Matrix4x4& view, const Matrix4x4& proj, const Matrix4x4& vp) const;
};
