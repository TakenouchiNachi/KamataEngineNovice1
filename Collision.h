#pragma once
#include "Math.h"

struct Segment {
    Vector3 origin;
    Vector3 diff;
};


inline Vector3 Project(const Vector3& v1, const Vector3& v2) {
    float t = Dot(v1, v2) / Dot(v2, v2);
    return Multiply(v2, t);
}

inline Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
    Vector3 rel = Subtract(point, segment.origin);
    float t = Dot(rel, segment.diff) / Dot(segment.diff, segment.diff);
    t = fmaxf(0.0f, fminf(1.0f, t));
    return Add(segment.origin, Multiply(segment.diff, t));
}

inline bool SphereVsSphere(const Vector3& centerA, float radiusA, const Vector3& centerB, float radiusB) {
    Vector3 diff = Subtract(centerA, centerB);
    float distSq = Dot(diff, diff);
    float radiusSum = radiusA + radiusB;
    return distSq <= (radiusSum * radiusSum);
}

inline bool SphereVsPlane(const Vector3& sphereCenter, float sphereRadius, const Vector3& planePoint, Vector3 planeNormal) {
    // 正規化
    float len = sqrtf(Dot(planeNormal, planeNormal));
    if (len != 0.0f) {
        planeNormal = Multiply(planeNormal, 1.0f / len);
    }

    Vector3 centerToPlane = Subtract(sphereCenter, planePoint);
    float distance = Dot(centerToPlane, planeNormal);
    return fabsf(distance) <= sphereRadius;
}