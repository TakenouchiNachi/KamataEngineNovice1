#include <Novice.h>
#include "Math.h"
#include "Camera.h"
#include "Grid.h"
#include "RenderContext.h"
#include "Sphere.h"
#include <imgui.h>
#include <cmath>

const char kWindowTitle[] = "Pendulum Simulation";

struct Pendulum {
    Vector3 anchor;              // 固定点
    float length;                // 糸の長さ
    float angle;                 // 現在の角度
    float angularVelocity;       // 角速度
    float angularAcceleration;   // 角加速度
};

struct Ball {
    Vector3 position;
    float radius;
    unsigned int color;
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    Novice::Initialize(kWindowTitle, 1280, 720);
    char keys[256]{}, preKeys[256]{};

    Grid grid;
    Camera camera;
    RenderContext renderContext;

    // 振り子と初期化
    Pendulum pendulum{};
    pendulum.anchor = { 0.0f, 1.0f, 0.0f };
    pendulum.length = 0.8f;
    pendulum.angle = 0.7f;
    pendulum.angularVelocity = 0.0f;
    pendulum.angularAcceleration = 0.0f;

    Ball ball{};
    ball.radius = 0.05f;
    ball.color = 0xFFFFFFFF;
    Sphere sphere{};
    sphere.radius = ball.radius;

    float deltaTime = 1.0f / 60.0f;
    bool isMoving = false;

    auto Update = [&]() {
        camera.Update();
        grid.Update();
        renderContext.Update(camera, 1280.0f, 720.0f);
        };

    while (Novice::ProcessMessage() == 0) {
        Novice::BeginFrame();
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        Update();

        // UI
        ImGui::Begin("Pendulum");
        if (ImGui::Button("Start")) isMoving = true;
        ImGui::End();

        if (isMoving) {
            // 加速度の計算（単振り子の公式）
            pendulum.angularAcceleration = (-9.8f / pendulum.length) * std::sin(pendulum.angle);
            pendulum.angularVelocity += pendulum.angularAcceleration * deltaTime;
            pendulum.angle += pendulum.angularVelocity * deltaTime;
        }

        // 球の位置を更新（Z=0平面）
        ball.position.x = pendulum.anchor.x + std::sin(pendulum.angle) * pendulum.length;
        ball.position.y = pendulum.anchor.y - std::cos(pendulum.angle) * pendulum.length;
        ball.position.z = pendulum.anchor.z;

        // ===== 描画 =====
        grid.Draw(renderContext.view, renderContext.proj, renderContext.vp);

        // 球の描画
        sphere.transform.position = ball.position;
        sphere.Draw(renderContext.view, renderContext.proj, renderContext.vp);

        // アーム（振り子の糸）描画
        Vector3 screenAnchor = TransformCoord(pendulum.anchor, renderContext.vp);
        Vector3 screenBall = TransformCoord(ball.position, renderContext.vp);
        Novice::DrawLine(
            static_cast<int>(screenAnchor.x),
            static_cast<int>(screenAnchor.y),
            static_cast<int>(screenBall.x),
            static_cast<int>(screenBall.y),
            0xFFFFFFFF
        );

        camera.ShowImGuiUI();

        Novice::EndFrame();
        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) break;
    }

    Novice::Finalize();
    return 0;
}