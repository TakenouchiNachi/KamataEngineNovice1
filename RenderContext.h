#pragma once
#include "Math.h"
#include "Camera.h"

class RenderContext {
public:
    Matrix4x4 view;
    Matrix4x4 proj;
    Matrix4x4 vp;
    Matrix4x4 mvp;

    RenderContext();
    void Update(const Camera& camera, float screenWidth, float screenHeight);
};
