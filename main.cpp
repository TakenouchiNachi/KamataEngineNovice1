#include <Novice.h>
#include "Math.h"
#include "Collision.h"
#include "DebugUI.h"
#include "Camera.h"
#include "Sphere.h"
#include "Grid.h"
#include "Cube.h"
#include "Triangle.h"
#include "RenderContext.h"
#include <imgui.h>

const char kWindowTitle[] = "LC1A_15_タケノウチ_ナチ_タイトル";

void DrawSegment(const Segment& segment, const Matrix4x4& mvp, unsigned int color) {
    Vector3 start = segment.origin;
    Vector3 end = Add(segment.origin, segment.diff);
    Vector3 s = TransformVector(start, mvp);
    Vector3 e = TransformVector(end, mvp);
    Novice::DrawLine((int)s.x, (int)s.y, (int)e.x, (int)e.y, color);
}

void DrawPoint(const Vector3& point, const Matrix4x4& mvp, unsigned int color) {
    Vector3 screen = TransformVector(point, mvp);
    Novice::DrawBox((int)screen.x - 2, (int)screen.y - 2, 4, 4, 0.0f, color, kFillModeSolid);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    Novice::Initialize(kWindowTitle, 1280, 720);
    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

    Grid grid;
    Camera camera;
    RenderContext renderContext;
    Cube cubeA;
    Sphere sphere;

    // === オブジェクトの初期化
    cubeA.position = { -1, 0, 0 };
    sphere.transform.position = { 0, 0, 0 };
    sphere.radius = 1.0f;

    auto Update = [&]() {
        camera.Update();
        grid.Update();
        renderContext.Update(camera, 1280.0f, 720.0f);
    };

    Segment segment = {
        { -3.0f, 2.0f, 0.0f },  // origin
        { 6.0f, -4.0f, 0.0f }   // diff
    };

    while (Novice::ProcessMessage() == 0) {
        Novice::BeginFrame();
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        Update();

        // === 判定処理
        bool isHit = AABBvsSphere(cubeA, sphere);
        sphere.isColliding = isHit;

        // === グリッドの面法線（Transformを反映させる）
        Vector3 localUp = { 0, 1, 0 };
        Matrix4x4 model = grid.transform.GetMatrix();
        Vector3 planeNormal = TransformVector(localUp, model);
        Vector3 planePoint = grid.transform.position;

        // === 当たり判定

        // === 描画行列（線・点描画用：単位モデル）
        Matrix4x4 cubeModel = MakeAffineMatrix(cubeA.size, { 0,0,0 }, cubeA.position);        Matrix4x4 mvp = renderContext.GetMVP(cubeModel);
        cubeA.Draw(mvp, isHit ? 0xFF0000FF : 0xFFFFFFFF);
        sphere.Draw(renderContext);

        // === ImGui UI
        ImGui::Begin("Cube A");
        cubeA.ShowImGuiUI();
        ImGui::End();

        ImGui::Begin("Sphere A");
        sphere.ShowImGuiUI();
        ImGui::End();

        Novice::EndFrame();
        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) break;
    }

    Novice::Finalize();
    return 0;
}
