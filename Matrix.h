#pragma once
#include "Struct.h"

// 行列構造体
struct Matrix4x4 {
	float m[4][4];
};

namespace MatrixMath {

	// --- ベクトル計算 ---
	Vector3 Add(const Vector3& v1, const Vector3& v2);
	Vector3 Subtract(const Vector3& v1, const Vector3& v2);
	Vector3 Multiply(float scalar, const Vector3& v);
	Vector3 Cross(const Vector3& v1, const Vector3& v2);
	float Dot(const Vector3& v1, const Vector3& v2);
	float Length(const Vector3& v);
	Vector3 Normalize(const Vector3& v);

	// --- 行列計算 ---
	Matrix4x4 MakeIdentity();
	Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
	Matrix4x4 MakeScaleMatrix(const Vector3& scale);
	Matrix4x4 MakeRotateXMatrix(float radian);
	Matrix4x4 MakeRotateYMatrix(float radian);
	Matrix4x4 MakeRotateZMatrix(float radian);

	// 任意軸回転行列
	Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle);

	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	// 透視投影・ビューポート
	Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
	Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

	// 行列演算
	Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
	Matrix4x4 Inverse(const Matrix4x4& m);

	// 座標変換
	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

	// --- 衝突判定 --- (★これを復活させました)
	bool IsCollision(const Triangle& triangle, const Segment& segment);

	// --- 描画関数など ---
	void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);
	void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color);
	void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color);

}