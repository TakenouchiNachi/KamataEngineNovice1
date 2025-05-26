#pragma once
#include "Math.h"
#include "Cube.h"
#include "Sphere.h"

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

inline bool SegmentVsPlane(const Segment& segment, const Vector3& planePoint, const Vector3& planeNormal, Vector3* outIntersect = nullptr) {
    float d = Dot(segment.diff, planeNormal);
    if (fabsf(d) < 1e-6f) return false; // 平行

    float t = Dot(Subtract(planePoint, segment.origin), planeNormal) / d;
    if (t < 0.0f || t > 1.0f) return false; // 範囲外

    if (outIntersect) {
        *outIntersect = Add(segment.origin, Multiply(segment.diff, t));
    }

    return true;
}

inline bool SegmentVsTriangle_CrossMethod(
    const Segment& segment,
    const Vector3& v0,
    const Vector3& v1,
    const Vector3& v2,
    Vector3* outIntersection = nullptr
) {
    // 平面法線
    Vector3 edge1 = Subtract(v1, v0);
    Vector3 edge2 = Subtract(v2, v0);
    Vector3 normal = Normalize(Cross(edge1, edge2));

    // 平面との交点を求める（SegmentVsPlaneの一部）
    float d = Dot(segment.diff, normal);
    if (fabsf(d) < 1e-6f) return false; // 平行

    float t = Dot(Subtract(v0, segment.origin), normal) / d;
    if (t < 0.0f || t > 1.0f) return false; // 線分の範囲外

    // 交点
    Vector3 p = Add(segment.origin, Multiply(segment.diff, t));
    if (outIntersection) *outIntersection = p;

    // 内外判定（クロス積）
    Vector3 v01 = Subtract(v1, v0);
    Vector3 v12 = Subtract(v2, v1);
    Vector3 v20 = Subtract(v0, v2);
    Vector3 v0p = Subtract(p, v0);
    Vector3 v1p = Subtract(p, v1);
    Vector3 v2p = Subtract(p, v2);

    Vector3 c01 = Cross(v01, v1p);
    Vector3 c12 = Cross(v12, v2p);
    Vector3 c20 = Cross(v20, v0p);

    if (Dot(c01, normal) >= 0.0f &&
        Dot(c12, normal) >= 0.0f &&
        Dot(c20, normal) >= 0.0f) {
        return true;
    }

    return false;
}

inline bool AABBvsAABB(const Cube& a, const Cube& b) {
    return (a.GetMin().x <= b.GetMax().x && a.GetMax().x >= b.GetMin().x) &&
        (a.GetMin().y <= b.GetMax().y && a.GetMax().y >= b.GetMin().y) &&
        (a.GetMin().z <= b.GetMax().z && a.GetMax().z >= b.GetMin().z);
}

inline bool AABBvsSphere(const Cube& cube, const Sphere& sphere) {
    Vector3 closestPoint = {
        std::fmax(cube.GetMin().x, std::fmin(sphere.transform.position.x, cube.GetMax().x)),
        std::fmax(cube.GetMin().y, std::fmin(sphere.transform.position.y, cube.GetMax().y)),
        std::fmax(cube.GetMin().z, std::fmin(sphere.transform.position.z, cube.GetMax().z))
    };

    Vector3 diff = Subtract(sphere.transform.position, closestPoint);
    return Dot(diff, diff) <= (sphere.radius * sphere.radius);
}