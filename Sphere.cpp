#include "Sphere.h"
#include <imgui.h>

Sphere::Sphere() {
    transform.position = { 0, 0, 0 };
}

void Sphere::Update() {
}

void Sphere::ShowImGuiUI() {
    ImGui::Begin("Sphere");
    ImGui::DragFloat3("Position", &transform.position.x, 0.1f);
    ImGui::DragFloat3("Rotation", &transform.rotation.x, 0.01f);
    ImGui::DragFloat("Radius", &radius, 0.01f, 0.1f, 10.0f);
    ImGui::End();
}

void Sphere::Draw(const RenderContext& ctx) const {
    // radius を行列でスケーリングに使う
    Matrix4x4 scale = {
        radius, 0, 0, 0,
        0, radius, 0, 0,
        0, 0, radius, 0,
        0, 0, 0, 1
    };
    Matrix4x4 model = Multiply(scale, transform.GetMatrix());
    Matrix4x4 mvp = ctx.GetMVP(model);
    int latCount = 10;
    int lonCount = 20;
    unsigned int color = isColliding ? 0xFF0000FF : 0xFFFFFFFF;

    for (int lat = 1; lat < latCount; ++lat) {
        float theta = 3.1415f * lat / latCount;
        for (int lon = 0; lon < lonCount; ++lon) {
            float phi1 = 2.0f * 3.1415f * lon / lonCount;
            float phi2 = 2.0f * 3.1415f * (lon + 1) / lonCount;

            Vector3 p1 = {
                sinf(theta) * cosf(phi1),
                cosf(theta),
                sinf(theta) * sinf(phi1)
            };
            Vector3 p2 = {
                sinf(theta) * cosf(phi2),
                cosf(theta),
                sinf(theta) * sinf(phi2)
            };
            Vector3 sp1 = TransformVector(p1, mvp);
            Vector3 sp2 = TransformVector(p2, mvp);
            Novice::DrawLine((int)sp1.x, (int)sp1.y, (int)sp2.x, (int)sp2.y, color);
        }
    }

    for (int lon = 0; lon < lonCount; ++lon) {
        float phi = 2.0f * 3.1415f * lon / lonCount;
        for (int lat = 0; lat < latCount; ++lat) {
            float theta1 = 3.1415f * lat / latCount;
            float theta2 = 3.1415f * (lat + 1) / latCount;

            Vector3 p1 = {
                sinf(theta1) * cosf(phi),
                cosf(theta1),
                sinf(theta1) * sinf(phi)
            };
            Vector3 p2 = {
                sinf(theta2) * cosf(phi),
                cosf(theta2),
                sinf(theta2) * sinf(phi)
            };
            Vector3 sp1 = TransformVector(p1, mvp);
            Vector3 sp2 = TransformVector(p2, mvp);
            Novice::DrawLine((int)sp1.x, (int)sp1.y, (int)sp2.x, (int)sp2.y, color);
        }
    }
}