#include "RenderContext.h"

RenderContext::RenderContext()
{
}

void RenderContext::Update(const Camera& camera, float screenWidth, float screenHeight) {
    view = camera.GetViewMatrix();
    proj = MakePerspectiveMatrix(3.1415f / 4.0f, screenWidth / screenHeight, 0.1f, 1000.0f);
    vp = MakeViewportMatrix(screenWidth, screenHeight);
}
