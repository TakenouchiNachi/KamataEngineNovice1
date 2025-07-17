#pragma once
#include "Math.h"

struct Spring {
    Vector3 anchor;
    float naturalLength;
    float stiffness;
    float dampingCoefficient;
};

struct Ball {
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    float mass;
    float radius;
    unsigned int color;
};

Vector3 ComputeForce(const Spring& spring, const Ball& ball);
void springDrawLine3D(
    const Vector3& p1,
    const Vector3& p2,
    unsigned int color,
    const Matrix4x4& view,
    const Matrix4x4& proj,
    const Matrix4x4& vp
);