#pragma once
#include "Math.h"
#include "Camera.h"

class RenderContext {
public:
    Matrix4x4 view;
    Matrix4x4 proj;
    Matrix4x4 vp;

    RenderContext();

    void Update(const Camera& camera, float screenWidth, float screenHeight);

    Matrix4x4 GetMVP(const Matrix4x4& model) const {
        return Multiply(Multiply(Multiply(model, view), proj), vp);
    }
};
