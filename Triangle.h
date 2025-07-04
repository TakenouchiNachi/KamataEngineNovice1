#pragma once
#include "Math.h"
#include "RenderContext.h"

class Triangle {
public:
    Transform transform;
    Triangle();

    void Draw(const RenderContext& rc, unsigned int color = 0xFFFFFFFF) const;
    void ShowImGuiUI();

    Vector3 GetWorldVertex(int index) const; // index = 0,1,2
private:
    Vector3 localVertices[3];
};