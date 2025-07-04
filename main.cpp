#include <Novice.h>
#include "Math.h"
#include "Camera.h"
#include "RenderContext.h"
#include <imgui.h>
#include <string>
#include "Grid.h"

const char kWindowTitle[] = "LC1A_15_タケノウチ_ナチ_タイトル";

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
    return Add(v1, Multiply(Subtract(v2, v1), t));
}

void DrawBezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Matrix4x4& vp, uint32_t color) {
    const int resolution = 32;
    for (int i = 0; i < resolution; ++i) {
        float t1 = (float)i / resolution;
        float t2 = (float)(i + 1) / resolution;

        Vector3 a1 = Lerp(p0, p1, t1);
        Vector3 a2 = Lerp(p1, p2, t1);
        Vector3 point1 = Lerp(a1, a2, t1);

        Vector3 b1 = Lerp(p0, p1, t2);
        Vector3 b2 = Lerp(p1, p2, t2);
        Vector3 point2 = Lerp(b1, b2, t2);

        Vector3 sp1 = TransformVector(point1, vp);
        Vector3 sp2 = TransformVector(point2, vp);

        Novice::DrawLine((int)sp1.x, (int)sp1.y, (int)sp2.x, (int)sp2.y, color);
    }
}

void DrawSphereMarker(const Vector3& worldPos, const Matrix4x4& vp, uint32_t color) {
    Vector3 screen = TransformVector(worldPos, vp);
    Novice::DrawEllipse((int)screen.x, (int)screen.y, 5, 5, 0.0f, color, kFillModeSolid);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    Novice::Initialize(kWindowTitle, 1280, 720);
    char keys[256] = {}, preKeys[256] = {};

    Camera camera;
    RenderContext renderContext;
    Grid grid;

    Vector3 controlPoints[3] = {
        {-0.8f, 0.58f, 1.0f},
        {1.76f, 1.0f, -0.3f},
        {0.94f, -0.7f, 2.3f},
    };

    auto Update = [&] {
        camera.Update();
        renderContext.Update(camera, 1280.0f, 720.0f);
        grid.Update();
        };

    while (Novice::ProcessMessage() == 0) {
        Novice::BeginFrame();
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        Update();
        Matrix4x4 vp = renderContext.vp;

        // ベジェ曲線
        DrawBezier(controlPoints[0], controlPoints[1], controlPoints[2], vp, 0xFFFFFFFF);

        // 制御点の描画とUI
        ImGui::Begin("Control Points");
        for (int i = 0; i < 3; ++i) {
            ImGui::DragFloat3(("P" + std::to_string(i)).c_str(), &controlPoints[i].x, 0.01f);
            DrawSphereMarker(controlPoints[i], vp, 0x000000FF);
        }
        ImGui::End();

        Novice::EndFrame();
        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) break;
    }

    Novice::Finalize();
    return 0;
}
