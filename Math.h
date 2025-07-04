#pragma once
#include <cmath>

struct Vector3 {
    float x, y, z;

    Vector3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
    Vector3& operator/=(float s) { x /= s; y /= s; z /= s; return *this; }
    Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
    Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
};

struct Matrix4x4 {
    float m[4][4];
};


// ベクトル・行列演算関数群（略：実装はプロジェクト内に含まれると想定）
float Dot(const Vector3& a, const Vector3& b);

Vector3 Add(const Vector3& a, const Vector3& b);

Vector3 Subtract(const Vector3& a, const Vector3& b);

Vector3 Multiply(const Vector3& v, float scalar);

Vector3 TransformVector(const Vector3& v, const Matrix4x4& m);

Matrix4x4 Multiply(const Matrix4x4& a, const Matrix4x4& b);

Matrix4x4 MakeRotateX(float angle);

Matrix4x4 MakeRotateY(float angle);

Matrix4x4 MakeRotateZ(float angle);

Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotation, Vector3 translation);

Matrix4x4 MakePerspectiveMatrix(float fovY, float aspect, float nearZ, float farZ);

Matrix4x4 MakeViewportMatrix(float width, float height);

Matrix4x4 Inverse(const Matrix4x4& m);

inline Vector3 operator+(const Vector3& v1, const Vector3& v2) {
    return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

inline Vector3 operator-(const Vector3& v1, const Vector3& v2) {
    return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

inline Vector3 operator*(float s, const Vector3& v) {
    return { s * v.x, s * v.y, s * v.z };
}

inline Vector3 operator*(const Vector3& v, float s) {
    return s * v;
}

inline Vector3 operator/(const Vector3& v, float s) {
    float inv = 1.0f / s;
    return { v.x * inv, v.y * inv, v.z * inv };
}

inline Vector3 operator*(const Vector3& v1, const Vector3& v2) {
    return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
}

inline Vector3 operator-(const Vector3& v) {
    return { -v.x, -v.y, -v.z };
}

inline Vector3 operator+(const Vector3& v) {
    return v;
}

inline Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2) {
    Matrix4x4 result{};
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result.m[i][j] = m1.m[i][j] + m2.m[i][j];
    return result;
}

inline Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2) {
    Matrix4x4 result{};
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result.m[i][j] = m1.m[i][j] - m2.m[i][j];
    return result;
}

inline Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
    Matrix4x4 result{};
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            result.m[row][col] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                result.m[row][col] += m1.m[row][k] * m2.m[k][col];
            }
        }
    }
    return result;
}

struct Transform {
    Vector3 position = { 0, 0, 0 };
    Vector3 rotation = { 0, 0, 0 };
    Vector3 scale = { 1, 1, 1 };
    Matrix4x4 GetMatrix() const { return MakeAffineMatrix(scale, rotation, position); }
};