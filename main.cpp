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

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    Novice::Initialize(kWindowTitle, 1280, 720);
    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

    Grid grid;
    Sphere sphereA;
    Camera camera;
    RenderContext renderContext;

    auto Update = [&]() {
        camera.Update();
        grid.Update();
        sphereA.Update();
        renderContext.Update(camera, 1280.0f, 720.0f);
    };

    //球の位置を初期化
    sphereA.transform.position = { 0,0,5 };
    sphereA.radius = 1.0f;

    while (Novice::ProcessMessage() == 0) {
        Novice::BeginFrame();
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        Update();

        sphereA.isColliding = SphereVsPlane(sphereA.transform.position, sphereA.radius, grid.planePoint, grid.planeNormal);

        grid.Draw(renderContext.view, renderContext.proj, renderContext.vp);
        sphereA.Draw(renderContext.view, renderContext.proj, renderContext.vp);

        camera.ShowImGuiUI();
        sphereA.ShowImGuiUI();

        Novice::EndFrame();
        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) break;
    }

    Novice::Finalize();
    return 0;
}
