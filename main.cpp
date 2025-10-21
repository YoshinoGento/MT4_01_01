#include <Novice.h>
#include "Struct.h"
#include "Matrix.h"
#include <cstdint>
#include <imgui.h>

const char kWindowTitle[] = "LE2C_28_ヨシノゲント_タイトル";




static const int kRowHeight = 20;
static const int kColumnWidth = 60;

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}


void MatrixScreenPrintf(int x, int y, const Matrix4x4& matirix, const char* label) {
	Novice::ScreenPrintf(x, y + 20, "%s", label);
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(x + column * kColumnWidth, y + (row + 1) * kRowHeight + 20,
				"%6.03f", matirix.m[row][column]);
		}
	}
}

// クォータニオンの表示関数
void QuaternionScreenPrintf(int x, int y, const Quaternion& q, const char* label, float t) {
	// ラベル部分（例: Slerp(q0, q1, 0.0f)）
	Novice::ScreenPrintf(x, y, "%s(t=%.1f)", label, t);

	// 数値部分（少し右にずらす）
	Novice::ScreenPrintf(x + 250, y, ":  %.02f, %.02f, %.02f, %.02f", q.x, q.y, q.z, q.w);
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	/*int kWindowWidth = 1280;
	int kWindowHeight = 720;*/







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
		/// 

		Quaternion rotation0 = MatrixMath::MakeRotateAxisAngleQuaternion({ 0.71f, 0.71f, 0.0f }, 0.3f);
		Quaternion rotation1 = MatrixMath::MakeRotateAxisAngleQuaternion({ 0.71f, 0.0f, 0.71f }, 3.141592f);

		Quaternion interpolate0 = MatrixMath::Slerp(rotation0, rotation1, 0.0f);
		Quaternion interpolate1 = MatrixMath::Slerp(rotation0, rotation1, 0.3f);
		Quaternion interpolate2 = MatrixMath::Slerp(rotation0, rotation1, 0.5f);
		Quaternion interpolate3 = MatrixMath::Slerp(rotation0, rotation1, 0.7f);
		Quaternion interpolate4 = MatrixMath::Slerp(rotation0, rotation1, 1.0f);


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		
		// クォータニオンの補間結果を表示
		QuaternionScreenPrintf(0, 100, interpolate0, "Slerp(q0, q1)", 0.0f);
		QuaternionScreenPrintf(0, 120, interpolate1, "Slerp(q0, q1)", 0.3f);
		QuaternionScreenPrintf(0, 140, interpolate2, "Slerp(q0, q1)", 0.5f);
		QuaternionScreenPrintf(0, 160, interpolate3, "Slerp(q0, q1)", 0.7f);
		QuaternionScreenPrintf(0, 180, interpolate4, "Slerp(q0, q1)", 1.0f);




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
