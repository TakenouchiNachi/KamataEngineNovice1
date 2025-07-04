#include <Novice.h>
#include "Math.h"
#include "Camera.h"
#include "RenderContext.h"
#include "Grid.h"
#include <imgui.h>
#include <string>

const char kWindowTitle[] = "Hierarchy Scene";

struct Object {
    Transform transform;
    Object* parent = nullptr;

    Matrix4x4 GetWorldMatrix() const {
        if (parent) {
            return Multiply(transform.GetMatrix(), parent->GetWorldMatrix());
        }
        return transform.GetMatrix();
    }
};

void DrawMarker(const Vector3& worldPos, const Matrix4x4& view, const Matrix4x4& proj, const Matrix4x4& vp, uint32_t color) {
    Matrix4x4 mvp = Multiply(Multiply(view, proj), vp);
    Vector3 screen = TransformVector(worldPos, mvp);
    Novice::DrawEllipse((int)screen.x, (int)screen.y, 5, 5, 0.0f, color, kFillModeSolid);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    Novice::Initialize(kWindowTitle, 1280, 720);
    char keys[256] = {}, preKeys[256] = {};

    Camera camera;
    RenderContext renderContext;
    Grid grid;

    Object root, childA, childB;
    root.transform.position = { 0, 0, 0 };
    childA.transform.position = { 0, 1, 0 };
    childB.transform.position = { 0, 1, 0 };

    childA.parent = &root;
    childB.parent = &childA;

    auto Update = [&]() {
        camera.Update();
        renderContext.Update(camera, 1280.0f, 720.0f);
        };

    while (Novice::ProcessMessage() == 0) {
        Novice::BeginFrame();
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        Update();

        Matrix4x4 view = renderContext.view;
        Matrix4x4 proj = renderContext.proj;
        Matrix4x4 vp = renderContext.vp;

        grid.Draw(view, proj, vp);

        Vector3 posRoot = { root.GetWorldMatrix().m[3][0], root.GetWorldMatrix().m[3][1], root.GetWorldMatrix().m[3][2] };
        Vector3 posChildA = { childA.GetWorldMatrix().m[3][0], childA.GetWorldMatrix().m[3][1], childA.GetWorldMatrix().m[3][2] };
        Vector3 posChildB = { childB.GetWorldMatrix().m[3][0], childB.GetWorldMatrix().m[3][1], childB.GetWorldMatrix().m[3][2] };

        DrawMarker(posRoot, view, proj, vp, 0xFFFF0000);
        DrawMarker(posChildA, view, proj, vp, 0x00FF00FF);
        DrawMarker(posChildB, view, proj, vp, 0x0000FFFF);

        ImGui::Begin("Root");
        ImGui::DragFloat3("Position", &root.transform.position.x, 0.01f);
        ImGui::End();

        ImGui::Begin("Child A");
        ImGui::DragFloat3("Position", &childA.transform.position.x, 0.01f);
        ImGui::End();

        ImGui::Begin("Child B");
        ImGui::DragFloat3("Position", &childB.transform.position.x, 0.01f);
        ImGui::End();

        Novice::EndFrame();
        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) break;
    }

    Novice::Finalize();
    return 0;
}
