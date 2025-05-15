#include <Novice.h>
#include "Math.h"
#include "Collision.h"
#include "DebugUI.h"
#include "Camera.h"
#include "Sphere.h"
#include "Grid.h"
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
    Sphere sphere;
    Camera camera;
    RenderContext renderContext;
    Segment segment = { {-2.0f, -1.0f, 0.0f}, {3.0f, 2.0f, 2.0f} };
    Vector3 point = { -1.5f, 0.6f, 0.6f };

    auto Update = [&]() {
        camera.Update();
        grid.Update();
        sphere.Update();
        renderContext.Update(camera, 1280.0f, 720.0f);
    };

    while (Novice::ProcessMessage() == 0) {
        Novice::BeginFrame();
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        Update();

        Vector3 closest = ClosestPoint(point, segment);
        Vector3 project = Project(Subtract(point, segment.origin), segment.diff);

        grid.Draw(renderContext.view, renderContext.proj, renderContext.vp);
        sphere.Draw(renderContext.view, renderContext.proj, renderContext.vp);
        DrawSegment(segment, renderContext.mvp, 0xFFFFFFFF);
        DrawPoint(point, renderContext.mvp, 0xFF0000FF);
        DrawPoint(closest, renderContext.mvp, 0x00FF00FF);

        camera.ShowImGuiUI();
        ShowSegmentPointUI(point, segment, closest);

        Novice::EndFrame();
        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) break;
    }

    Novice::Finalize();
    return 0;
}
