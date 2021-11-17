#pragma once

#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>

using namespace DirectX;

class Vector3:public XMFLOAT3 {
public:
	//コンストラクタ
	Vector3();
	Vector3(float x, float y, float z);

	//メンバ関数
	const float length();
	Vector3& normalize();
	const float dot(const Vector3& v);
	const Vector3 cross(const Vector3& v);

	//単項演算子オーバーロード
	const Vector3 operator+();
	const Vector3 operator-();

	//代入演算子オーバーロード
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);

};

//2項演算子オーバーロード
//※いろんな引数(引数の型と順序)のパターンに対応するため、以下のように準備している
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);

//補間関数
//線形補間(1次関数補間)
const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);
const Vector3 easeInQuad(const Vector3& start, const Vector3& end, const float t);
const Vector3 easeOutQuad(const Vector3& start, const Vector3& end, const float t);
const Vector3 easeInOutQuad(const Vector3& start, const Vector3& end, const float t);
const Vector3 easeInQuint(const Vector3& start, const Vector3& end, const float t);
const Vector3 easeOutQuint(const Vector3& start, const Vector3& end, const float t);
const Vector3 easeInOutQuint(const Vector3& start, const Vector3& end, const float t);