#include <Novice.h>
#include "math.h"
#include <cmath>
#include <imgui.h>

const char kWindowTitle[] = "LC1A_15_タケノウチ_ナチ_タイトル";

// ==== 最低限の構造定義 ====
struct Vector3 {
	float x, y, z;
};

struct Vector2 {
	float x, y;
};

struct Matrix4x4 {
	float m[4][4];
};

struct Transform {
	Vector3 position = { 0, 0, 0 };
	Vector3 rotation = { 0, 0, 0 };
	Vector3 scale = { 1, 1, 1 };

	Matrix4x4 GetMatrix() const;
};

// ==== 行列ユーティリティ関数 ====
Matrix4x4 Multiply(const Matrix4x4& a, const Matrix4x4& b) {
	Matrix4x4 result = {};
	for (int y = 0; y < 4; ++y) {
		for (int x = 0; x < 4; ++x) {
			result.m[y][x] = 0;
			for (int k = 0; k < 4; ++k) {
				result.m[y][x] += a.m[y][k] * b.m[k][x];
			}
		}
	}
	return result;
}

Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 inv = {};
	// 回転成分の転置（逆行列）
	for (int y = 0; y < 3; ++y) {
		for (int x = 0; x < 3; ++x) {
			inv.m[y][x] = m.m[x][y];
		}
	}
	// 平行移動の逆変換
	inv.m[3][0] = -(m.m[3][0] * inv.m[0][0] + m.m[3][1] * inv.m[1][0] + m.m[3][2] * inv.m[2][0]);
	inv.m[3][1] = -(m.m[3][0] * inv.m[0][1] + m.m[3][1] * inv.m[1][1] + m.m[3][2] * inv.m[2][1]);
	inv.m[3][2] = -(m.m[3][0] * inv.m[0][2] + m.m[3][1] * inv.m[1][2] + m.m[3][2] * inv.m[2][2]);
	inv.m[0][3] = inv.m[1][3] = inv.m[2][3] = 0;
	inv.m[3][3] = 1;
	return inv;
}

Vector3 TransformVector(const Vector3& v, const Matrix4x4& m) {
	Vector3 result;
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];
	result.x = (v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w;
	result.y = (v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w;
	result.z = (v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w;
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

Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotate, Vector3 translate) {
	Matrix4x4 rotX = MakeRotateX(rotate.x);
	Matrix4x4 rotY = MakeRotateY(rotate.y);
	Matrix4x4 rotZ = MakeRotateZ(rotate.z);
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
		translate.x, translate.y, translate.z, 1
	};

	return Multiply(Multiply(scaleMat, rot), transMat);
}

Matrix4x4 Transform::GetMatrix() const {
	return MakeAffineMatrix(scale, rotation, position);
}

Matrix4x4 MakeViewMatrix(Vector3 eye, Vector3 target, Vector3 up) {
	Vector3 z = { target.x - eye.x, target.y - eye.y, target.z - eye.z };
	float lenZ = sqrtf(z.x * z.x + z.y * z.y + z.z * z.z);
	z = { z.x / lenZ, z.y / lenZ, z.z / lenZ };

	Vector3 x = {
		up.y * z.z - up.z * z.y,
		up.z * z.x - up.x * z.z,
		up.x * z.y - up.y * z.x
	};
	float lenX = sqrtf(x.x * x.x + x.y * x.y + x.z * x.z);
	x = { x.x / lenX, x.y / lenX, x.z / lenX };

	Vector3 y = {
		z.y * x.z - z.z * x.y,
		z.z * x.x - z.x * x.z,
		z.x * x.y - z.y * x.x
	};

	Matrix4x4 view = {
		x.x, y.x, z.x, 0,
		x.y, y.y, z.y, 0,
		x.z, y.z, z.z, 0,
		-(x.x * eye.x + x.y * eye.y + x.z * eye.z),
		-(y.x * eye.x + y.y * eye.y + y.z * eye.z),
		-(z.x * eye.x + z.y * eye.y + z.z * eye.z),
		1
	};
	return view;
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

void DrawGrid(const Transform& transform, const Matrix4x4& view, const Matrix4x4& proj, const Matrix4x4& vp,
	int gridSize = 10, float spacing = 1.0f, unsigned int color = 0x888888FF) {
	Matrix4x4 model = transform.GetMatrix();
	Matrix4x4 mvp = Multiply(Multiply(Multiply(model, view), proj), vp);
	for (int i = -gridSize; i <= gridSize; ++i) {
		Vector3 startX = { (float)i * spacing, 0, -gridSize * spacing };
		Vector3 endX = { (float)i * spacing, 0,  gridSize * spacing };
		Vector3 startZ = { -gridSize * spacing, 0, (float)i * spacing };
		Vector3 endZ = { gridSize * spacing, 0, (float)i * spacing };
		Vector3 sX = TransformVector(startX, mvp);
		Vector3 eX = TransformVector(endX, mvp);
		Vector3 sZ = TransformVector(startZ, mvp);
		Vector3 eZ = TransformVector(endZ, mvp);
		Novice::DrawLine((int)sX.x, (int)sX.y, (int)eX.x, (int)eX.y, color);
		Novice::DrawLine((int)sZ.x, (int)sZ.y, (int)eZ.x, (int)eZ.y, color);
	}
}

void DrawSphere(const Transform& transform, const Matrix4x4& view, const Matrix4x4& proj, const Matrix4x4& vp,
	float radius = 1.0f, int latCount = 10, int lonCount = 20, unsigned int color = 0xFFFFFFFF) {
	Matrix4x4 model = transform.GetMatrix();
	Matrix4x4 mvp = Multiply(Multiply(Multiply(model, view), proj), vp);

	// 経度方向（横線）
	for (int lat = 1; lat < latCount; ++lat) {
		float theta = 3.1415f * lat / latCount;
		for (int lon = 0; lon < lonCount; ++lon) {
			float phi1 = 2.0f * 3.1415f * lon / lonCount;
			float phi2 = 2.0f * 3.1415f * (lon + 1) / lonCount;
			Vector3 p1 = {
				radius * sinf(theta) * cosf(phi1),
				radius * cosf(theta),
				radius * sinf(theta) * sinf(phi1)
			};
			Vector3 p2 = {
				radius * sinf(theta) * cosf(phi2),
				radius * cosf(theta),
				radius * sinf(theta) * sinf(phi2)
			};
			Vector3 sp1 = TransformVector(p1, mvp);
			Vector3 sp2 = TransformVector(p2, mvp);
			Novice::DrawLine((int)sp1.x, (int)sp1.y, (int)sp2.x, (int)sp2.y, color);
		}
	}

	// 緯度方向（縦線）
	for (int lon = 0; lon < lonCount; ++lon) {
		float phi = 2.0f * 3.1415f * lon / lonCount;
		for (int lat = 0; lat < latCount; ++lat) {
			float theta1 = 3.1415f * lat / latCount;
			float theta2 = 3.1415f * (lat + 1) / latCount;
			Vector3 p1 = {
				radius * sinf(theta1) * cosf(phi),
				radius * cosf(theta1),
				radius * sinf(theta1) * sinf(phi)
			};
			Vector3 p2 = {
				radius * sinf(theta2) * cosf(phi),
				radius * cosf(theta2),
				radius * sinf(theta2) * sinf(phi)
			};
			Vector3 sp1 = TransformVector(p1, mvp);
			Vector3 sp2 = TransformVector(p2, mvp);
			Novice::DrawLine((int)sp1.x, (int)sp1.y, (int)sp2.x, (int)sp2.y, color);
		}
	}
}
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Novice::Initialize(kWindowTitle, 1280, 720);
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Transform gridTransform;
	gridTransform.rotation = { 0.1f, 0.2f, 0.0f };

	Transform sphereTransform;
	sphereTransform.position = { 0, 0, 3 };

	Transform cameraTransform;
	cameraTransform.position = { 0, 0, -5 }; // 初期視点を少し後退

	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		sphereTransform.rotation.y += 0.02f;

		// カメラ → view行列をTransformから逆行列として構築
		Matrix4x4 cameraMatrix = cameraTransform.GetMatrix();
		Matrix4x4 view = Inverse(cameraMatrix);
		Matrix4x4 proj = MakePerspectiveMatrix(3.1415f / 4.0f, 1280.0f / 720.0f, 0.1f, 100.0f);
		Matrix4x4 vp = MakeViewportMatrix(1280.0f, 720.0f);

		ImGui::Begin("Camera");
		ImGui::DragFloat3("Position", &cameraTransform.position.x, 0.1f);
		ImGui::DragFloat3("Rotation", &cameraTransform.rotation.x, 0.01f);
		ImGui::End();

		DrawGrid(gridTransform, view, proj, vp);
		DrawSphere(sphereTransform, view, proj, vp);

		Novice::EndFrame();
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) break;
	}
	Novice::Finalize();
	return 0;
}
