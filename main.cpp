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

	
	Quaternion rotation = MatrixMath::MakeRotateAxisAngleQuaternion(
		MatrixMath::Normalize(Vector3{ 1.0f,0.4f,-0.2f }), 0.45f);
	Vector3 pointY = { 2.1f,-0.9f,1.3f };
	




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

		Matrix4x4 rotateMatrix = MatrixMath::MakeRotateMatrix(rotation);
		Vector3 rotateByQuaternion = MatrixMath::RotateVector(pointY, rotation);
		Vector3 rotateByMatrix = MatrixMath::Transform(pointY, rotateMatrix);


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		QuaternionScreenPrintf(0, kRowHeight * 0, rotation, "rotation");
		MatrixScreenPrintf(0, kRowHeight * 1, rotateMatrix, "  rotateMatrix :");
		VectorScreenPrintf(0, kRowHeight * 7, rotateByQuaternion, ": rotateByQuaternion");
		VectorScreenPrintf(0, kRowHeight * 8, rotateByMatrix, ": rotateByMatrix");
	 


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
