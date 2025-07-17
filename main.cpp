#include <Novice.h>
#include "Math.h"
#include "Camera.h"
#include "Grid.h"
#include "RenderContext.h"
#include "Sphere.h"
#include <imgui.h>
#include <cmath>

const char kWindowTitle[] = "Uniform Circular Motion";

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

    // Ballと描画用Sphere
    Ball ball{
        {0.8f, 0.0f, 0.0f}, // 初期位置（X = 半径）
        0.05f,
        0x0000FFFF // 青
    };
    Sphere ballSphere;
    ballSphere.radius = ball.radius;

    // 円運動設定
    const float angularVelocity = 3.14159f; // π rad/s（2秒で1周）
    const float radius = 0.8f;
    float angle = 0.0f;

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
        float deltaTime = 1.0f / 60.0f;

        // UI
        ImGui::Begin("Spring Simulation");
        if (ImGui::Button("Start")) isMoving = true;
        ImGui::End();

        // 円運動シミュレーション
        if (isMoving) {
            angle += angularVelocity * deltaTime;
            if (angle > 2 * 3.14159f) angle -= 2 * 3.14159f;
            ball.position.x = std::cos(angle) * radius;
            ball.position.y = std::sin(angle) * radius;
            ball.position.z = 0.0f; // XY平面上
        }

        // ===== 描画 =====
        grid.Draw(renderContext.view, renderContext.proj, renderContext.vp);

        ballSphere.transform.position = ball.position;
        ballSphere.Draw(renderContext.view, renderContext.proj, renderContext.vp);

        camera.ShowImGuiUI();

        Novice::EndFrame();
        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) break;
    }

    Novice::Finalize();
    return 0;
}