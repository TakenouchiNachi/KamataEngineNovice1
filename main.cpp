#include <Novice.h>
#include "Math.h"
#include "Collision.h"
#include "DebugUI.h"
#include "Camera.h"
#include "Sphere.h"
#include "Grid.h"
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
    Triangle triangle;

    auto Update = [&]() {
        camera.Update();
        grid.Update();
        renderContext.Update(camera, 1280.0f, 720.0f);
    };

    Segment segment = {
        { -3.0f, 2.0f, 0.0f },  // origin
        { 6.0f, -4.0f, 0.0f }   // diff
    };

    Vector3 intersect;

    while (Novice::ProcessMessage() == 0) {
        Novice::BeginFrame();
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        Update();

        // === 判定処理
        Vector3 v0 = triangle.GetWorldVertex(0);
        Vector3 v1 = triangle.GetWorldVertex(1);
        Vector3 v2 = triangle.GetWorldVertex(2);

        // === グリッドの面法線（Transformを反映させる）
        Vector3 localUp = { 0, 1, 0 };
        Matrix4x4 model = grid.transform.GetMatrix();
        Vector3 planeNormal = TransformVector(localUp, model);
        Vector3 planePoint = grid.transform.position;

        // === 当たり判定
        bool isHit = SegmentVsTriangle_CrossMethod(segment,
            triangle.GetWorldVertex(0),
            triangle.GetWorldVertex(1),
            triangle.GetWorldVertex(2),
            &intersect);

        // === 描画行列（線・点描画用：単位モデル）
        Matrix4x4 identityMVP = renderContext.GetMVP(
            MakeAffineMatrix({ 1,1,1 }, { 0,0,0 }, { 0,0,0 }));

        // === 三角形描画（赤：ヒット時）
        triangle.Draw(renderContext, isHit ? 0xFF0000FF : 0xFFFFFFFF);

        // === 線分描画（常時白）
        DrawSegment(segment, identityMVP, 0xFFFFFFFF);

        // === 当たっていれば交点を描画
        if (isHit) {
            DrawPoint(intersect, identityMVP, 0x00FF00FF);
        }

        // === ImGui UI
        ImGui::Begin("Triangle");
        triangle.ShowImGuiUI(); // 内部で position/rotation 操作
        ImGui::End();

        ImGui::Begin("Segment");
        ImGui::DragFloat3("Origin", &segment.origin.x, 0.1f);
        ImGui::DragFloat3("Diff", &segment.diff.x, 0.1f);
        ImGui::End();

        Novice::EndFrame();
        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) break;
    }

    Novice::Finalize();
    return 0;
}
