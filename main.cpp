#include <Novice.h>
#include "Struct.h"
#include "Matrix.h"
#include <cstdint>

const char kWindowTitle[] = "LE2C_28_ヨシノゲント_タイトル";

// 数値表示用のヘルパー関数
static const int kColumnWidth = 60;
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

// 行列表示用のヘルパー関数
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			Novice::ScreenPrintf(x + col * kColumnWidth, y + (row + 1) * 20, "%6.03f", matrix.m[row][col]);
		}
	}
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// --- 初期化 ---

	// カメラ（全体が見える位置に固定）
	Vector3 cameraTranslate = { 0.0f, 1.9f, -6.4f };
	Vector3 cameraRotate = { 0.26f, 0.0f, 0.0f };

	// セグメント（線分）
	Segment segment = {
		{ -2.0f, -1.0f, 0.0f }, // origin
		{ 3.0f, 2.0f, 2.0f }    // diff
	};

	// 三角形
	Triangle triangle = {
		Vector3{ -1.0f, 0.0f, 0.0f },
		Vector3{ 0.0f, 2.0f, 0.0f },
		Vector3{ 1.0f, 0.0f, 0.0f }
	};

	// ★課題：任意軸回転のパラメータ
	Vector3 rotateAxis = { 1.0f, 1.0f, 1.0f };
	// 軸を正規化
	rotateAxis = MatrixMath::Normalize(rotateAxis);
	float rotateAngle = 0.44f;

	// 回転行列の計算
	Matrix4x4 rotateMatrix = MatrixMath::MakeRotateAxisAngle(rotateAxis, rotateAngle);


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		// --- 更新処理 ---

		// 衝突判定
		bool isColliding = MatrixMath::IsCollision(triangle, segment);
		uint32_t triangleColor = isColliding ? 0xFF0000FF : 0xFFFFFFFF; // 赤 or 白


		// --- 描画処理 ---

		// 行列計算
		Matrix4x4 cameraMatrix = MatrixMath::MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = MatrixMath::Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MatrixMath::MakePerspectiveFovMatrix(0.45f, 1280.0f / 720.0f, 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = MatrixMath::Multiply(viewMatrix, projectionMatrix);
		Matrix4x4 viewportMatrix = MatrixMath::MakeViewportMatrix(0, 0, 1280, 720, 0, 1);

		// グリッド・オブジェクト描画
		MatrixMath::DrawGrid(viewProjectionMatrix, viewportMatrix);
		MatrixMath::DrawTriangle(triangle, viewProjectionMatrix, viewportMatrix, triangleColor);
		MatrixMath::DrawSegment(segment, viewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);

		// 数値表示

		// ★課題の表示を追加
		Novice::ScreenPrintf(0, 0, "MakeRotateAxisAngle"); // タイトル
		MatrixScreenPrintf(0, 20, rotateMatrix, "rotateMatrix");

		// フレームの終了
		Novice::EndFrame();

		// ESCキーで終了
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}