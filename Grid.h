#pragma once
#include "Math.h"
#include"Novice.h"
#include"RenderContext.h"

class Grid {
public:
    Transform transform;
    Grid();
    void Update();
    Vector3 planePoint = { 0, 0, 0 };
    Vector3 planeNormal = { 0.0f, 0.0f, 0.0f };
    void Draw(const RenderContext& rc) const;
};
