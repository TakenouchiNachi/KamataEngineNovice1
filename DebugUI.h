#pragma once
#include "Math.h"
#include "Collision.h"
#include <imgui.h>

inline void ShowSegmentPointUI(Vector3& point, Segment& segment, Vector3& closest) {
    ImGui::Begin("Segment Debug");
    ImGui::DragFloat3("Point", &point.x, 0.1f);
    ImGui::DragFloat3("Segment Origin", &segment.origin.x, 0.1f);
    ImGui::DragFloat3("Segment Diff", &segment.diff.x, 0.1f);
    ImGui::InputFloat3("Closest", &closest.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
    ImGui::End();
}
