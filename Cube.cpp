#include "Cube.h"

void Cube::ShowImGuiUI() {
    ImGui::DragFloat3("Position", &position.x, 0.1f);
    ImGui::DragFloat3("Size", &size.x, 0.1f, 0.0f, 100.0f);
}

void Cube::Draw(const Matrix4x4& mvp, unsigned int color) const {
    Vector3 min = GetMin();
    Vector3 max = GetMax();
    Vector3 corners[8] = {
        {min.x, min.y, min.z}, {max.x, min.y, min.z},
        {max.x, max.y, min.z}, {min.x, max.y, min.z},
        {min.x, min.y, max.z}, {max.x, min.y, max.z},
        {max.x, max.y, max.z}, {min.x, max.y, max.z},
    };

    auto drawLine = [&](int i, int j) {
        Vector3 a = TransformVector(corners[i], mvp);
        Vector3 b = TransformVector(corners[j], mvp);
        Novice::DrawLine((int)a.x, (int)a.y, (int)b.x, (int)b.y, color);
        };

    drawLine(0, 1); drawLine(1, 2); drawLine(2, 3); drawLine(3, 0);
    drawLine(4, 5); drawLine(5, 6); drawLine(6, 7); drawLine(7, 4);
    drawLine(0, 4); drawLine(1, 5); drawLine(2, 6); drawLine(3, 7);
}