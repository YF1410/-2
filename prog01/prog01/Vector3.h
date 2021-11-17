#pragma once

#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>

using namespace DirectX;

class Vector3:public XMFLOAT3 {
public:
	//�R���X�g���N�^
	Vector3();
	Vector3(float x, float y, float z);

	//�����o�֐�
	const float length();
	Vector3& normalize();
	const float dot(const Vector3& v);
	const Vector3 cross(const Vector3& v);

	//�P�����Z�q�I�[�o�[���[�h
	const Vector3 operator+();
	const Vector3 operator-();

	//������Z�q�I�[�o�[���[�h
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);

};

//2�����Z�q�I�[�o�[���[�h
//�������Ȉ���(�����̌^�Ə���)�̃p�^�[���ɑΉ����邽�߁A�ȉ��̂悤�ɏ������Ă���
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);

//��Ԋ֐�
//���`���(1���֐����)
const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);
const Vector3 easeInQuad(const Vector3& start, const Vector3& end, const float t);
const Vector3 easeOutQuad(const Vector3& start, const Vector3& end, const float t);
const Vector3 easeInOutQuad(const Vector3& start, const Vector3& end, const float t);
const Vector3 easeInQuint(const Vector3& start, const Vector3& end, const float t);
const Vector3 easeOutQuint(const Vector3& start, const Vector3& end, const float t);
const Vector3 easeInOutQuint(const Vector3& start, const Vector3& end, const float t);