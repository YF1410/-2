#pragma once

#include <Windows.h>
#include <DirectXMath.h>
#include <d3d12.h>
#include <d3dx12.h>
#include "Object3d.h"
#include "Model.h"
#include "Input.h"

class player
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	Object3d* PlayerObj = nullptr;
	Model* PlayerModel = nullptr;
	Input* input = nullptr;
public: // ƒƒ“ƒoŠÖ”
	void Initialize();
	

	

	void PlayerMove(Object3d* object, Input* input);
	// –ˆƒtƒŒ[ƒ€ˆ—
	void Update();
	void Draw();
private:
	
	XMFLOAT3 PlayerPos = PlayerObj->GetPosition();
	XMFLOAT3 PlayerRotation = { 0,0,0 };
};

