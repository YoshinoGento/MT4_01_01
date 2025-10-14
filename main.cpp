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
void QuaternionScreenPrintf(int x, int y, const Quaternion& q, const char* label) {
	Novice::ScreenPrintf(x, y, "%s", label);
	Novice::ScreenPrintf(x + 120, y, ":  %.02f, %.02f, %.02f, %.02f", q.x, q.y, q.z, q.w);
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

	
	Quaternion q1 = { 2.0f,3.0f,4.0f,1.0f };
	Quaternion q2 = { 1.0f,3.0f,5.0f,2.0f };
	Quaternion identity = MatrixMath::IdentityQuaternion();
	Quaternion conj = MatrixMath::Conjugate(q1);
	Quaternion inv = MatrixMath::Inverse(q1);
	Quaternion normal = MatrixMath::Normalize(q1);
	Quaternion mul1 = MatrixMath::Multiply(q1, q2);
	Quaternion mul2 = MatrixMath::Multiply(q2, q1);
	float  norm = MatrixMath::Norm(q1);




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

		

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		

	 // ===== 描画処理 =====
		int x = 50;
		int y = 60;
		int step = 25;

		Novice::ScreenPrintf(x, y - 30, "Quaternion Calculation Results");

		QuaternionScreenPrintf(x, y + step * 0, q1, "q1");
		QuaternionScreenPrintf(x, y + step * 1, q2, "q2");
		QuaternionScreenPrintf(x, y + step * 2, identity, "Identity");
		QuaternionScreenPrintf(x, y + step * 3, conj, "Conjugate(q1)");
		QuaternionScreenPrintf(x, y + step * 4, inv, "Inverse(q1)");
		QuaternionScreenPrintf(x, y + step * 5, normal, "Normalize(q1)");
		QuaternionScreenPrintf(x, y + step * 6, mul1, "q1 × q2");
		QuaternionScreenPrintf(x, y + step * 7, mul2, "q2 × q1");

		Novice::ScreenPrintf(x, y + step * 9, "Norm(q1) : %.02f", norm);


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
