#include "Math.h"
#include <cmath>

float Dot(const Vector3& a, const Vector3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Add(const Vector3& a, const Vector3& b) {
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}

Vector3 Subtract(const Vector3& a, const Vector3& b) {
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}

Vector3 Multiply(const Vector3& v, float scalar) {
    return { v.x * scalar, v.y * scalar, v.z * scalar };
}

Vector3 Cross(const Vector3& a, const Vector3& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

Vector3 Normalize(const Vector3& v) {
    float len = sqrtf(Dot(v, v));
    if (len == 0.0f) return { 0, 0, 0 };
    return Multiply(v, 1.0f / len);
}

Vector3 TransformVector(const Vector3& v, const Matrix4x4& m) {
    float x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
    float y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
    float z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];
    float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

    if (w == 0.0f) w = 1.0f; // 念のため
    return { x / w, y / w, z / w };
}

Matrix4x4 Multiply(const Matrix4x4& a, const Matrix4x4& b) {
    Matrix4x4 result = {};
    for (int y = 0; y < 4; ++y)
        for (int x = 0; x < 4; ++x)
            for (int k = 0; k < 4; ++k)
                result.m[y][x] += a.m[y][k] * b.m[k][x];
    return result;
}

Matrix4x4 MakeRotateX(float angle) {
    return {
        1, 0, 0, 0,
        0, cosf(angle), sinf(angle), 0,
        0, -sinf(angle), cosf(angle), 0,
        0, 0, 0, 1
    };
}

Matrix4x4 MakeRotateY(float angle) {
    return {
        cosf(angle), 0, -sinf(angle), 0,
        0, 1, 0, 0,
        sinf(angle), 0, cosf(angle), 0,
        0, 0, 0, 1
    };
}

Matrix4x4 MakeRotateZ(float angle) {
    return {
        cosf(angle), sinf(angle), 0, 0,
        -sinf(angle), cosf(angle), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
}

Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotation, Vector3 translation) {
    Matrix4x4 rotX = MakeRotateX(rotation.x);
    Matrix4x4 rotY = MakeRotateY(rotation.y);
    Matrix4x4 rotZ = MakeRotateZ(rotation.z);
    Matrix4x4 rot = Multiply(Multiply(rotX, rotY), rotZ); 

    Matrix4x4 scaleMat = {
        scale.x, 0, 0, 0,
        0, scale.y, 0, 0,
        0, 0, scale.z, 0,
        0, 0, 0, 1
    };

    Matrix4x4 transMat = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        translation.x, translation.y, translation.z, 1
    };

    return Multiply(Multiply(scaleMat, rot), transMat);
}

Matrix4x4 MakePerspectiveMatrix(float fovY, float aspect, float nearZ, float farZ) {
    float f = 1.0f / tanf(fovY / 2.0f);
    Matrix4x4 mat = {};
    mat.m[0][0] = f / aspect;
    mat.m[1][1] = f;
    mat.m[2][2] = farZ / (farZ - nearZ);
    mat.m[2][3] = 1.0f;
    mat.m[3][2] = (-nearZ * farZ) / (farZ - nearZ);
    return mat;
}

Matrix4x4 MakeViewportMatrix(float width, float height) {
    Matrix4x4 mat = {};
    mat.m[0][0] = width / 2.0f;
    mat.m[1][1] = -height / 2.0f;
    mat.m[3][0] = width / 2.0f;
    mat.m[3][1] = height / 2.0f;
    mat.m[2][2] = 1.0f;
    mat.m[3][3] = 1.0f;
    return mat;
}

Matrix4x4 Inverse(const Matrix4x4& m) {
    Matrix4x4 inv = {};
    for (int y = 0; y < 3; ++y)
        for (int x = 0; x < 3; ++x)
            inv.m[y][x] = m.m[x][y];
    inv.m[3][0] = -(m.m[3][0] * inv.m[0][0] + m.m[3][1] * inv.m[1][0] + m.m[3][2] * inv.m[2][0]);
    inv.m[3][1] = -(m.m[3][0] * inv.m[0][1] + m.m[3][1] * inv.m[1][1] + m.m[3][2] * inv.m[2][1]);
    inv.m[3][2] = -(m.m[3][0] * inv.m[0][2] + m.m[3][1] * inv.m[1][2] + m.m[3][2] * inv.m[2][2]);
    inv.m[0][3] = inv.m[1][3] = inv.m[2][3] = 0;
    inv.m[3][3] = 1;
    return inv;
}
