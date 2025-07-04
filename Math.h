#pragma once
#include <cmath>

struct Vector3 {
    float x, y, z;
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

struct Transform {
    Vector3 position = { 0, 0, 0 };
    Vector3 rotation = { 0, 0, 0 };
    Vector3 scale = { 1, 1, 1 };
    Matrix4x4 GetMatrix() const { return MakeAffineMatrix(scale, rotation, position); }
};
