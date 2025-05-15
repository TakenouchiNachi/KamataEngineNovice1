#include "Camera.h"
#include <imgui.h>


Camera::Camera() {
    transform.position = { 0, 0, -5 };
}

void Camera::ShowImGuiUI() {
    ImGui::Begin("Camera");
    ImGui::DragFloat3("Position", &transform.position.x, 0.1f);
    ImGui::DragFloat3("Rotation", &transform.rotation.x, 0.01f);
    ImGui::End();
}

Matrix4x4 Camera::GetViewMatrix() const {
    return Inverse(transform.GetMatrix());
}

//　マウス操作でカメラを操作できるようにする（BlenderやUnity参考に）
void Camera::CameraControl() {
    // === ホイールで前後移動 ===
    int wheel = Novice::GetWheel();
    if (wheel != 0) {
        Vector3 forward = { 0, 0, 1 };
        transform.position = Add(transform.position, Multiply(forward, wheel * 0.001f));
    }

    // === マウス座標の取得 ===
    static int prevMouseX = 0;
    static int prevMouseY = 0;
    int mouseX, mouseY;
    Novice::GetMousePosition(&mouseX, &mouseY);
    int dx = mouseX - prevMouseX;
    int dy = mouseY - prevMouseY;

    // === 左クリック：回転 ===
    if (Novice::IsPressMouse(0)) {
        transform.rotation.y += dx * 0.01f;
        transform.rotation.x += dy * 0.01f;
    }

    // === 右クリック：パン移動 ===
    if (Novice::IsPressMouse(1)) {
        // スクリーンX軸を右、Y軸を上とした移動
        transform.position.x -= dx * 0.01f;
        transform.position.y += dy * 0.01f;
    }

    prevMouseX = mouseX;
    prevMouseY = mouseY;
}

void Camera::Update() {
    CameraControl();
}
