#include "Spring.h"
#include "Math.h"
#include "Novice.h"

Vector3 ComputeForce(const Spring& spring, const Ball& ball) {
    Vector3 diff = ball.position - spring.anchor;
    float length = Length(diff);

    if (length == 0.0f) return { 0.0f, 0.0f, 0.0f };

    Vector3 direction = Normalize(diff);
    Vector3 restPosition = spring.anchor + direction * spring.naturalLength;
    Vector3 displacement = ball.position - restPosition;
    Vector3 restoringForce = -spring.stiffness * displacement;
    Vector3 dampingForce = -spring.dampingCoefficient * ball.velocity;

    return restoringForce + dampingForce;
}

void springDrawLine3D(
    const Vector3& p1,
    const Vector3& p2,
    unsigned int color,
    const Matrix4x4& view,
    const Matrix4x4& proj,
    const Matrix4x4& vp
) {
    Matrix4x4 mvp = view * proj * vp;
    Vector3 screenP1 = TransformVector(p1, mvp);
    Vector3 screenP2 = TransformVector(p2, mvp);

    Novice::DrawLine(
        static_cast<int>(screenP1.x), static_cast<int>(screenP1.y),
        static_cast<int>(screenP2.x), static_cast<int>(screenP2.y),
        color
    );
}