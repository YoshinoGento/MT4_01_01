#pragma once  
#include "Struct.h"  
#include <cstdint> // uint32_t 用
#include <cassert> // assert 用

// 4x4 行列の定義
struct Matrix4x4 {
    float m[4][4];
};

// 数学関数群
namespace MatrixMath {

    // --- ベクトル演算 ---
    Vector3 Add(const Vector3& v1, const Vector3& v2);       // ベクトル加算
    Vector3 Subtract(const Vector3& v1, const Vector3& v2);  // ベクトル減算
    Vector3 Cross(const Vector3& v1, const Vector3& v2);     // 外積
    float Dot(const Vector3& v1, const Vector3& v2);         // 内積
    Vector3 MultiplyV(float scalar, const Vector3& v);       // スカラー倍
    float Length(const Vector3& v);                          // ベクトル長さ
    Vector3 Normalize(const Vector3& v);                     // 正規化
    Vector3 Project(const Vector3& v1, const Vector3& v2);   // v1をv2へ射影
    Vector3 Perpendicular(const Vector3& v);                 // 任意の垂直ベクトルを返す

    // --- 行列生成 ---
    Matrix4x4 MakeIdentity4x4();                                                // 単位行列
    Matrix4x4 MakeScaleMatrix(const Vector3& scale);                         // 拡大縮小
    Matrix4x4 MakeRotateXMatrix(float radian);                               // X軸回転
    Matrix4x4 MakeRotateYMatrix(float radian);                               // Y軸回転
    Matrix4x4 MakeRotateZMatrix(float radian);                               // Z軸回転
    Matrix4x4 MakeRotateAxisMatrix(const Vector3& axis, float angle);        // 任意軸回転
    Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to);  // ある方向→別方向への回転行列
    Matrix4x4 MakeTranslateMatrix(const Vector3& translate);                 // 平行移動
    Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate); // アフィン行列

    // --- 投影・変換 ---
    Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspect, float nearClip, float farClip); // 透視投影
    Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip); // 正射影
    Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);    // ビューポート変換
    Matrix4x4 MultiplyM(const Matrix4x4& m1, const Matrix4x4& m2);            // 行列同士の積
    Matrix4x4 Inverse(const Matrix4x4& m);                                    // 逆行列
    Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);        // 座標変換

    // --- 描画系 ---
    void DrawSphere(const Sphere& sphere, const Matrix4x4& vp, const Matrix4x4& viewport, uint32_t color);
    void DrawGrid(const Matrix4x4& vp, const Matrix4x4& viewport);
    void DrawSegment(const Segment& segment, const Matrix4x4& vp, const Matrix4x4& viewport, uint32_t color);
    //void DrawPlane(const Plane& plane, const Matrix4x4& vp, const Matrix4x4& viewport, uint32_t color);
    void DrawTriangle(const Triangle& tri, const Matrix4x4& vp, const Matrix4x4& viewport, uint32_t color);

    // --- 衝突判定 ---
    bool IsCollisionP(const Segment& segment, const Plane& plane);            // 平面と線分
    bool IsCollisionT(const Triangle& triangle, const Segment& segment);      // 三角形と線分
    Vector3 ClosestPoint(const Vector3& point, const Segment& segment);       // 点と線分の最近接点


    //Quaternionの積
	Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);

    //単位Quaternionを返す
    Quaternion IdentityQuaternion();

	//共役Quaternionを返す
	Quaternion Conjugate(const Quaternion& quaternion);

    //Quaternionのnormを返す
    float Norm(const Quaternion& quaternion);

	//正規化したQuaternionを返す
	Quaternion Normalize(const Quaternion& quaternion);

	//逆Quaternionを返す
	Quaternion Inverse(const Quaternion& quaternion);

	//任意軸回転Quaternionを表すQuaternionの生成
    Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);

	//ベクトルをQuaternionで回転させた結果のベクトルを求める
    Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);

	//Quaternionから回転行列を生成する
    Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);

	//球面線形補間(Slerp)
	Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);





};
