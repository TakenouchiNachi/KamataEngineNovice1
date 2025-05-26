#include "Triangle.h"
#include <imgui.h>
#include <Novice.h>

Triangle::Triangle() {
    transform.position = { 0, 0, 0 };
    localVertices[0] = { 0.0f, 1.0f, 0.0f };
    localVertices[1] = { -1.0f, -1.0f, 0.0f };
    localVertices[2] = { 1.0f, -1.0f, 0.0f };
}

Vector3 Triangle::GetWorldVertex(int index) const {
    Matrix4x4 model = transform.GetMatrix();
    return TransformVector(localVertices[index], model);
}

void Triangle::Draw(const RenderContext& rc, unsigned int color) const {
    Matrix4x4 model = transform.GetMatrix();
    Matrix4x4 mvp = rc.GetMVP(model);

    Vector3 v0 = TransformVector(localVertices[0], mvp);
    Vector3 v1 = TransformVector(localVertices[1], mvp);
    Vector3 v2 = TransformVector(localVertices[2], mvp);

    Novice::DrawLine((int)v0.x, (int)v0.y, (int)v1.x, (int)v1.y, color);
    Novice::DrawLine((int)v1.x, (int)v1.y, (int)v2.x, (int)v2.y, color);
    Novice::DrawLine((int)v2.x, (int)v2.y, (int)v0.x, (int)v0.y, color);
}

void Triangle::ShowImGuiUI() {
    ImGui::Begin("Triangle");
    ImGui::DragFloat3("Position", &transform.position.x, 0.1f);
    ImGui::DragFloat3("Rotation", &transform.rotation.x, 0.01f);
    ImGui::End();
}