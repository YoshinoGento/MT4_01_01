#pragma once

// ベクトル
struct Vector3 {
	float x;
	float y;
	float z;
};

// 線分
struct Segment {
	Vector3 origin; //!< 始点
	Vector3 diff;   //!< 終点への差分ベクトル
};

// 三角形
struct Triangle {
	Vector3 vertices[3]; //!< 3つの頂点
};