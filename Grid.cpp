#include "Grid.h"
#include<imgui.h>


Grid::Grid() {
    transform.rotation = { 0.0f, 0.0f, 0.0f };
}

void Grid::Update() {
    // 静的
}

void Grid::Draw(const RenderContext& rc) const {
    Matrix4x4 model = transform.GetMatrix();
    Matrix4x4 mvp = rc.GetMVP(model);

    for (int i = -10; i <= 10; ++i) {
        // X軸に平行
        Vector3 startX = { -10.0f, 0.0f, (float)i };
        Vector3 endX = { 10.0f, 0.0f, (float)i };

        // Z軸に平行
        Vector3 startZ = { (float)i, 0.0f, -10.0f };
        Vector3 endZ = { (float)i, 0.0f,  10.0f };

        Vector3 sX = TransformVector(startX, mvp);
        Vector3 eX = TransformVector(endX, mvp);
        Vector3 sZ = TransformVector(startZ, mvp);
        Vector3 eZ = TransformVector(endZ, mvp);

        auto SafeLine = [](const Vector3& a, const Vector3& b) {
            return std::isfinite(a.x) && std::isfinite(a.y) &&
                std::isfinite(b.x) && std::isfinite(b.y);
            };

        if (SafeLine(sX, eX)) Novice::DrawLine((int)sX.x, (int)sX.y, (int)eX.x, (int)eX.y, 0x888888FF);
        if (SafeLine(sZ, eZ)) Novice::DrawLine((int)sZ.x, (int)sZ.y, (int)eZ.x, (int)eZ.y, 0x888888FF);
    }
}