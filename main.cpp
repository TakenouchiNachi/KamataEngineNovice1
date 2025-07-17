#include <Novice.h>
#include "Math.h"
#include "Collision.h"
#include "DebugUI.h"
#include "Camera.h"
#include "Grid.h"
#include "RenderContext.h"
#include "Spring.h"
#include "Sphere.h"
#include <imgui.h>

const char kWindowTitle[] = "Spring Simulation";

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    Novice::Initialize(kWindowTitle, 1280, 720);
    char keys[256]{}, preKeys[256]{};

    Grid grid;
    Camera camera;
    RenderContext renderContext;

    // SpringとBallの初期化
    Spring spring{
        {0.0f, 0.0f, 0.0f},  // アンカー位置
        1.0f,                // 自然長
        100.0f,              // 剛性
        2.0f                 // 減衰係数
    };
    Ball ball{
        {1.2f, 0.0f, 0.0f},  // 位置
        {}, {},              // 速度・加速度
        2.0f,                // 質量
        0.05f,               // 半径
        0x0000FFFF           // 色（青）
    };

    // 描画用Sphere
    Sphere anchorSphere;
    Sphere ballSphere;

    bool isSimulating = false;

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

        // UI表示
        ImGui::Begin("Spring Simulation");
        if (ImGui::Button("Start")) isSimulating = true;
        ImGui::DragFloat3("Ball Position", &ball.position.x, 0.01f);
        ImGui::DragFloat3("Anchor", &spring.anchor.x, 0.01f);
        ImGui::DragFloat("Natural Length", &spring.naturalLength, 0.01f, 0.0f, 10.0f);
        ImGui::DragFloat("Stiffness", &spring.stiffness, 1.0f, 0.0f, 500.0f);
        ImGui::DragFloat("Damping", &spring.dampingCoefficient, 0.1f, 0.0f, 50.0f);
        ImGui::End();

        // シミュレーション処理
        if (isSimulating) {
            Vector3 force = ComputeForce(spring, ball);
            ball.acceleration = force / ball.mass;
            ball.velocity += ball.acceleration * deltaTime;
            ball.position += ball.velocity * deltaTime;
        }

        // ===== 描画 =====
        // グリッド
        grid.Draw(renderContext.view, renderContext.proj, renderContext.vp);

        // スプリングの線
        springDrawLine3D(spring.anchor, ball.position, 0xFFFFFFFF,
            renderContext.view, renderContext.proj, renderContext.vp);

        // Sphere描画（アンカー）
        anchorSphere.transform.position = spring.anchor;
        anchorSphere.radius = 0.03f;
        anchorSphere.Draw(renderContext.view, renderContext.proj, renderContext.vp);

        // Sphere描画（ボール）
        ballSphere.transform.position = ball.position;
        ballSphere.radius = ball.radius;
        ballSphere.Draw(renderContext.view, renderContext.proj, renderContext.vp);

        // カメラ操作
        camera.ShowImGuiUI();

        Novice::EndFrame();
        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) break;
    }

    Novice::Finalize();
    return 0;
}