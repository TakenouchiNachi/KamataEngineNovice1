#pragma once
#include "Math.h"
#include <imgui.h>
#include <Novice.h>

class Cube {
public:
    Vector3 position{ 0, 0, 0 };
    Vector3 size{ 1, 1, 1 }; // 半サイズではなく実サイズ

    void ShowImGuiUI();

    // AABBの最小・最大座標を返す
    Vector3 GetMin() const {
        return {
            position.x - size.x / 2,
            position.y - size.y / 2,
            position.z - size.z / 2
        };
    }

    Vector3 GetMax() const {
        return {
            position.x + size.x / 2,
            position.y + size.y / 2,
            position.z + size.z / 2
        };
    }

    void Draw(const Matrix4x4& mvp, unsigned int color) const;
};