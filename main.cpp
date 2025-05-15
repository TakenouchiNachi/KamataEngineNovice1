#include <Novice.h>
#include"math.h"
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

// ==== 行列計算ユーティリティ ====
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

Vector3 Transform(const Vector3& v, const Matrix4x4& m) {
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
	// 回転行列を個別に合成
	Matrix4x4 rotX = MakeRotateX(rotate.x);
	Matrix4x4 rotY = MakeRotateY(rotate.y);
	Matrix4x4 rotZ = MakeRotateZ(rotate.z);
	Matrix4x4 rot = Multiply(Multiply(rotX, rotY), rotZ);

	// スケール
	Matrix4x4 scaleMat = {
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1
	};

	// 移動
	Matrix4x4 transMat = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		translate.x, translate.y, translate.z, 1
	};

	// 最終アフィン行列
	return Multiply(Multiply(scaleMat, rot), transMat);
}

Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 inv = {};

	// 回転部分（3x3）を転置（逆行列）
	for (int y = 0; y < 3; ++y) {
		for (int x = 0; x < 3; ++x) {
			inv.m[y][x] = m.m[x][y];
		}
	}

	// 平行移動の逆算
	inv.m[3][0] = -(m.m[3][0] * inv.m[0][0] + m.m[3][1] * inv.m[1][0] + m.m[3][2] * inv.m[2][0]);
	inv.m[3][1] = -(m.m[3][0] * inv.m[0][1] + m.m[3][1] * inv.m[1][1] + m.m[3][2] * inv.m[2][1]);
	inv.m[3][2] = -(m.m[3][0] * inv.m[0][2] + m.m[3][1] * inv.m[1][2] + m.m[3][2] * inv.m[2][2]);

	inv.m[0][3] = inv.m[1][3] = inv.m[2][3] = 0;
	inv.m[3][3] = 1;

	return inv;
}

// ==== カメラ行列定義 ====
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
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};


	// ==== 三角形ローカル頂点（原点中心） ====
	Vector3 p0 = { -0.5f, -0.5f, 0.0f };
	Vector3 p1 = { +0.5f, -0.5f, 0.0f };
	Vector3 p2 = { 0.0f, +0.5f, 0.0f };

	// ==== 回転アニメーション（ポリゴン側） ====
	static Vector3 rotateAngle = { 0, 0, 0 };
	rotateAngle.y += 0.02f; // Y軸回転のみ。自由に調整可

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
	// ==== 三角形の回転アニメーション ====
		rotateAngle.y += 0.02f;

		// ==== ポリゴンのアフィン行列（原点回転＋Z=3へ） ====
		Matrix4x4 model = MakeAffineMatrix({ 1,1,1 }, rotateAngle, { 0, 0, 3 });

		// ==== カメラは固定（位置0, 向きZ+, 上Y+） ====
		Matrix4x4 view = MakeViewMatrix({ 0, 0, 0 }, { 0, 0, 1 }, { 0, 1, 0 });
		Matrix4x4 proj = MakePerspectiveMatrix(3.1415f / 4.0f, 1280.0f / 720.0f, 0.1f, 100.0f);
		Matrix4x4 vp = MakeViewportMatrix(1280.0f, 720.0f);

		// ==== モデル→ビュー→プロジェクション→ビューポート ====
		Matrix4x4 mvp = Multiply(Multiply(Multiply(model, view), proj), vp);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		// ==== 各頂点をスクリーン座標へ変換 ====
		Vector3 sp0 = Transform(p0, mvp);
		Vector3 sp1 = Transform(p1, mvp);
		Vector3 sp2 = Transform(p2, mvp);

		// ==== Noviceで三角形描画 ====
		Novice::DrawTriangle(
			(int)sp0.x, (int)sp0.y,
			(int)sp1.x, (int)sp1.y,
			(int)sp2.x, (int)sp2.y,
			RED, kFillModeSolid);
		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
