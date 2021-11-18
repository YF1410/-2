#pragma once

#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include "Object3d.h"
#include "DebugText.h"
#include "Audio.h"
#include "Model.h"
#include "stdlib.h"
#include "time.h"
#include "Vector3.h"

using namespace DirectX;

class GameScene {
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // 静的メンバ変数
	static const int debugTextTexNumber = 0;

public: // メンバ関数
	// コンストクラタ
	GameScene();
	// デストラクタ
	~GameScene();
	// 初期化
	void Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio);
	// 毎フレーム処理
	void Update();
	//pos初期化
	void resetPos();
	// 描画
	void Draw();
	//移動
	void Move();
	//回避
	void avoidance();

private: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* titleAudio = nullptr;
	Audio* playAudio = nullptr;
	Audio* gameoverAudio = nullptr;
	Audio* carAudio = nullptr;
	Audio* dangerAudio = nullptr;
	Audio* deadAudio = nullptr;
	DebugText debugText;

	// ゲームシーン用
	Sprite* sprite = nullptr;
	Sprite* back1 = nullptr;
	Sprite* back2 = nullptr;
	Sprite* back3 = nullptr;
	Sprite* title = nullptr;
	Sprite* uiBack = nullptr;
	Sprite* gameOver = nullptr;
	Model* playerModel = nullptr;
	Model* groundModel = nullptr;
	Object3d* playerObj = nullptr;
	Object3d* groundObj = nullptr;
	XMFLOAT3 startPlayerPos;
	XMFLOAT3 endPlayerPos;
	XMFLOAT3 startCameraEye;
	XMFLOAT3 endCameraEye;
	XMFLOAT3 startCameraTarget;
	XMFLOAT3 endCameraTarget;
	int nowScene = 0;
	float playerScale = 1.0f;
	float groundScale = 15.0f;
	float largeCarScale = 3.0f;
	float scrollGround = 35.0f;
	float nowTime = 0;
	float endTime = 0.5;
	float timeRate = 0;
	int avoidChange = 0;
	float moveAmount = 1.0f;
	float avoidMove = 40.0f;
};