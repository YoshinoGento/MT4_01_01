#include "Matrix.h"
#include <Novice.h>
#include <cmath>
#include <algorithm>

namespace MatrixMath {

	// --- ベクトル計算 ---
	Vector3 Add(const Vector3& v1, const Vector3& v2) {
		return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	}

	Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
		return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	}

	Vector3 Multiply(float scalar, const Vector3& v) {
		return { v.x * scalar, v.y * scalar, v.z * scalar };
	}

	Vector3 Cross(const Vector3& v1, const Vector3& v2) {
		return {
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x
		};
	}

	float Dot(const Vector3& v1, const Vector3& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	float Length(const Vector3& v) {
		return std::sqrt(Dot(v, v));
	}

	Vector3 Normalize(const Vector3& v) {
		float len = Length(v);
		if (len != 0) {
			return { v.x / len, v.y / len, v.z / len };
		}
		return v;
	}

	// --- 行列計算 ---
	Matrix4x4 MakeIdentity() {
		Matrix4x4 result = {};
		result.m[0][0] = 1.0f;
		result.m[1][1] = 1.0f;
		result.m[2][2] = 1.0f;
		result.m[3][3] = 1.0f;
		return result;
	}

	Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
		Matrix4x4 result = MakeIdentity();
		result.m[3][0] = translate.x;
		result.m[3][1] = translate.y;
		result.m[3][2] = translate.z;
		return result;
	}

	Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
		Matrix4x4 result = MakeIdentity();
		result.m[0][0] = scale.x;
		result.m[1][1] = scale.y;
		result.m[2][2] = scale.z;
		return result;
	}

	Matrix4x4 MakeRotateXMatrix(float radian) {
		Matrix4x4 result = MakeIdentity();
		result.m[1][1] = std::cos(radian);
		result.m[1][2] = std::sin(radian);
		result.m[2][1] = -std::sin(radian);
		result.m[2][2] = std::cos(radian);
		return result;
	}

	Matrix4x4 MakeRotateYMatrix(float radian) {
		Matrix4x4 result = MakeIdentity();
		result.m[0][0] = std::cos(radian);
		result.m[0][2] = -std::sin(radian);
		result.m[2][0] = std::sin(radian);
		result.m[2][2] = std::cos(radian);
		return result;
	}

	Matrix4x4 MakeRotateZMatrix(float radian) {
		Matrix4x4 result = MakeIdentity();
		result.m[0][0] = std::cos(radian);
		result.m[0][1] = std::sin(radian);
		result.m[1][0] = -std::sin(radian);
		result.m[1][1] = std::cos(radian);
		return result;
	}

	// 任意軸回転行列
	Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle) {
		float c = std::cos(angle);
		float s = std::sin(angle);

		Matrix4x4 result = {}; // ゼロクリア

		result.m[0][0] = axis.x * axis.x * (1 - c) + c;
		result.m[0][1] = axis.x * axis.y * (1 - c) + axis.z * s;
		result.m[0][2] = axis.x * axis.z * (1 - c) - axis.y * s;
		result.m[0][3] = 0.0f;

		result.m[1][0] = axis.x * axis.y * (1 - c) - axis.z * s;
		result.m[1][1] = axis.y * axis.y * (1 - c) + c;
		result.m[1][2] = axis.y * axis.z * (1 - c) + axis.x * s;
		result.m[1][3] = 0.0f;

		result.m[2][0] = axis.x * axis.z * (1 - c) + axis.y * s;
		result.m[2][1] = axis.y * axis.z * (1 - c) - axis.x * s;
		result.m[2][2] = axis.z * axis.z * (1 - c) + c;
		result.m[2][3] = 0.0f;

		result.m[3][0] = 0.0f;
		result.m[3][1] = 0.0f;
		result.m[3][2] = 0.0f;
		result.m[3][3] = 1.0f;

		return result;
	}

	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
		Matrix4x4 scaleMat = MakeScaleMatrix(scale);
		Matrix4x4 rotateXMat = MakeRotateXMatrix(rotate.x);
		Matrix4x4 rotateYMat = MakeRotateYMatrix(rotate.y);
		Matrix4x4 rotateZMat = MakeRotateZMatrix(rotate.z);
		Matrix4x4 translateMat = MakeTranslateMatrix(translate);

		Matrix4x4 rotateMat = Multiply(rotateXMat, Multiply(rotateYMat, rotateZMat));
		return Multiply(Multiply(scaleMat, rotateMat), translateMat);
	}

	// 透視投影・ビューポート
	Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
		Matrix4x4 result = {};
		float tanHalfFov = std::tan(fovY / 2.0f);
		result.m[0][0] = 1.0f / (aspectRatio * tanHalfFov);
		result.m[1][1] = 1.0f / tanHalfFov;
		result.m[2][2] = farClip / (farClip - nearClip);
		result.m[2][3] = 1.0f;
		result.m[3][2] = -(farClip * nearClip) / (farClip - nearClip);
		return result;
	}

	Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
		Matrix4x4 result = {};
		result.m[0][0] = width / 2.0f;
		result.m[1][1] = -height / 2.0f;
		result.m[2][2] = maxDepth - minDepth;
		result.m[3][0] = left + width / 2.0f;
		result.m[3][1] = top + height / 2.0f;
		result.m[3][2] = minDepth;
		result.m[3][3] = 1.0f;
		return result;
	}

	// 行列演算
	Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
		Matrix4x4 result = {};
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				for (int k = 0; k < 4; ++k) {
					result.m[i][j] += m1.m[i][k] * m2.m[k][j];
				}
			}
		}
		return result;
	}

	Matrix4x4 Inverse(const Matrix4x4& m) {
		float A = m.m[0][0] * m.m[1][1] - m.m[0][1] * m.m[1][0];
		float B = m.m[0][0] * m.m[1][2] - m.m[0][2] * m.m[1][0];
		float C = m.m[0][0] * m.m[1][3] - m.m[0][3] * m.m[1][0];
		float D = m.m[0][1] * m.m[1][2] - m.m[0][2] * m.m[1][1];
		float E = m.m[0][1] * m.m[1][3] - m.m[0][3] * m.m[1][1];
		float F = m.m[0][2] * m.m[1][3] - m.m[0][3] * m.m[1][2];

		float G = m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0];
		float H = m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0];
		float I = m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0];
		float J = m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1];
		float K = m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1];
		float L = m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2];

		float det = A * L - B * K + C * J + D * I - E * H + F * G;
		Matrix4x4 result = {};

		if (det != 0.0f) {
			float invDet = 1.0f / det;
			result.m[0][0] = (m.m[1][1] * L - m.m[1][2] * K + m.m[1][3] * J) * invDet;
			result.m[0][1] = (-m.m[0][1] * L + m.m[0][2] * K - m.m[0][3] * J) * invDet;
			result.m[0][2] = (m.m[3][1] * F - m.m[3][2] * E + m.m[3][3] * D) * invDet;
			result.m[0][3] = (-m.m[2][1] * F + m.m[2][2] * E - m.m[2][3] * D) * invDet;

			result.m[1][0] = (-m.m[1][0] * L + m.m[1][2] * I - m.m[1][3] * H) * invDet;
			result.m[1][1] = (m.m[0][0] * L - m.m[0][2] * I + m.m[0][3] * H) * invDet;
			result.m[1][2] = (-m.m[3][0] * F + m.m[3][2] * C - m.m[3][3] * B) * invDet;
			result.m[1][3] = (m.m[2][0] * F - m.m[2][2] * C + m.m[2][3] * B) * invDet;

			result.m[2][0] = (m.m[1][0] * K - m.m[1][1] * I + m.m[1][3] * G) * invDet;
			result.m[2][1] = (-m.m[0][0] * K + m.m[0][1] * I - m.m[0][3] * G) * invDet;
			result.m[2][2] = (m.m[3][0] * E - m.m[3][1] * C + m.m[3][3] * A) * invDet;
			result.m[2][3] = (-m.m[2][0] * E + m.m[2][1] * C - m.m[2][3] * A) * invDet;

			result.m[3][0] = (-m.m[1][0] * J + m.m[1][1] * H - m.m[1][2] * G) * invDet;
			result.m[3][1] = (m.m[0][0] * J - m.m[0][1] * H + m.m[0][2] * G) * invDet;
			result.m[3][2] = (-m.m[3][0] * D + m.m[3][1] * B - m.m[3][2] * A) * invDet;
			result.m[3][3] = (m.m[2][0] * D - m.m[2][1] * B + m.m[2][2] * A) * invDet;
		}
		return result;
	}

	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
		Vector3 result;
		result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0];
		result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1];
		result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2];
		float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + matrix.m[3][3];
		if (w != 0.0f) {
			result.x /= w;
			result.y /= w;
			result.z /= w;
		}
		return result;
	}

	// --- 衝突判定 --- (★ここを復活させました)
	bool IsCollision(const Triangle& triangle, const Segment& segment) {
		// 三角形の平面の法線ベクトルを計算
		Vector3 v1 = Subtract(triangle.vertices[1], triangle.vertices[0]);
		Vector3 v2 = Subtract(triangle.vertices[2], triangle.vertices[0]);
		Vector3 normal = Cross(v1, v2);
		normal = Normalize(normal);

		// 線分の方向ベクトル
		Vector3 dir = segment.diff;

		// 平面と線分の内積
		float dot = Dot(normal, dir);

		// 線分と平面が平行なら衝突しない
		if (std::abs(dot) < 1.0e-5f) {
			return false;
		}

		// 線分と平面の交点tを求める
		Vector3 p0 = triangle.vertices[0];
		Vector3 vOriginToP0 = Subtract(segment.origin, p0);
		float t = -Dot(vOriginToP0, normal) / dot;

		// tが0〜1の範囲外なら線分は平面に届いていない
		if (t < 0.0f || t > 1.0f) {
			return false;
		}

		// 平面上の交点
		Vector3 p = Add(segment.origin, Multiply(t, dir));

		// 三角形の内外判定 (各辺とのクロス積で判定)
		Vector3 edge0 = Subtract(triangle.vertices[1], triangle.vertices[0]);
		Vector3 edge1 = Subtract(triangle.vertices[2], triangle.vertices[1]);
		Vector3 edge2 = Subtract(triangle.vertices[0], triangle.vertices[2]);

		Vector3 c0 = Subtract(p, triangle.vertices[0]);
		Vector3 c1 = Subtract(p, triangle.vertices[1]);
		Vector3 c2 = Subtract(p, triangle.vertices[2]);

		Vector3 cross0 = Cross(edge0, c0);
		Vector3 cross1 = Cross(edge1, c1);
		Vector3 cross2 = Cross(edge2, c2);

		if (Dot(cross0, normal) >= 0.0f &&
			Dot(cross1, normal) >= 0.0f &&
			Dot(cross2, normal) >= 0.0f) {
			return true;
		}

		return false;
	}

	// --- 描画関数 ---
	static const int kGridSubdivision = 10;
	static const float kGridWidth = 10.0f;

	void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
		const float kGridHalfWidth = kGridWidth / 2.0f;
		const float kGridInterval = kGridWidth / kGridSubdivision;

		for (int i = 0; i <= kGridSubdivision; ++i) {
			float x = -kGridHalfWidth + (i * kGridInterval);
			Vector3 startZ = { x, 0.0f, -kGridHalfWidth }; Vector3 endZ = { x, 0.0f, kGridHalfWidth };
			DrawSegment({ startZ, Subtract(endZ, startZ) }, viewProjectionMatrix, viewportMatrix, (i == kGridSubdivision / 2) ? 0x000000FF : 0xAAAAAAFF);

			float z = -kGridHalfWidth + (i * kGridInterval);
			Vector3 startX = { -kGridHalfWidth, 0.0f, z }; Vector3 endX = { kGridHalfWidth, 0.0f, z };
			DrawSegment({ startX, Subtract(endX, startX) }, viewProjectionMatrix, viewportMatrix, (i == kGridSubdivision / 2) ? 0x000000FF : 0xAAAAAAFF);
		}
	}

	void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color) {
		Vector3 start = segment.origin;
		Vector3 end = Add(segment.origin, segment.diff);
		Vector3 screenStart = Transform(Transform(start, viewProjectionMatrix), viewportMatrix);
		Vector3 screenEnd = Transform(Transform(end, viewProjectionMatrix), viewportMatrix);
		Novice::DrawLine((int)screenStart.x, (int)screenStart.y, (int)screenEnd.x, (int)screenEnd.y, color);
	}

	void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color) {
		Vector3 screenVertices[3];
		for (int i = 0; i < 3; ++i) {
			screenVertices[i] = Transform(Transform(triangle.vertices[i], viewProjectionMatrix), viewportMatrix);
		}
		Novice::DrawTriangle(
			(int)screenVertices[0].x, (int)screenVertices[0].y,
			(int)screenVertices[1].x, (int)screenVertices[1].y,
			(int)screenVertices[2].x, (int)screenVertices[2].y,
			color, kFillModeSolid
		);
	}
}