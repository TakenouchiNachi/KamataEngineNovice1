#include "Grid.h"

Grid::Grid() {
    transform.rotation = { 0.1f, 0.2f, 0.0f };
}

void Grid::Update() {
    // 静的
}

void Grid::Draw(const Matrix4x4& view, const Matrix4x4& proj, const Matrix4x4& vp) const {
    Matrix4x4 model = transform.GetMatrix();
    Matrix4x4 mvp = Multiply(Multiply(Multiply(model, view), proj), vp);

    for (int i = -10; i <= 10; ++i) {
        Vector3 startX = { (float)i, 0, -10 };
        Vector3 endX = { (float)i, 0,  10 };
        Vector3 startZ = { -10, 0, (float)i };
        Vector3 endZ = { 10, 0, (float)i };

        Vector3 sX = TransformVector(startX, mvp);
        Vector3 eX = TransformVector(endX, mvp);
        Vector3 sZ = TransformVector(startZ, mvp);
        Vector3 eZ = TransformVector(endZ, mvp);

        Novice::DrawLine((int)sX.x, (int)sX.y, (int)eX.x, (int)eX.y, 0x888888FF);
        Novice::DrawLine((int)sZ.x, (int)sZ.y, (int)eZ.x, (int)eZ.y, 0x888888FF);
    }

}
