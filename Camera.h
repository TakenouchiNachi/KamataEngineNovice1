#pragma once
#include "Math.h"
#include "Novice.h"

class Camera {
public:
    Transform transform;
    Camera();
    Matrix4x4 GetViewMatrix() const;
    void ShowImGuiUI();
    void CameraControl();
    void Update();
};
