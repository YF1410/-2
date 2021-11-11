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

	void Move();

private: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* titleAudio = nullptr;
	Audio *playAudio = nullptr;
	Audio *gameoverAudio = nullptr;
	Audio *carAudio = nullptr;
	Audio *dangerAudio = nullptr;
	Audio *deadAudio = nullptr;
	DebugText debugText;

	// ゲームシーン用
	Sprite* sprite = nullptr;
	Sprite* back1 = nullptr;
	Sprite* back2 = nullptr;
	Sprite* back3 = nullptr;
	Sprite *title = nullptr;
	Sprite *uiBack = nullptr;
	Sprite *gameOver = nullptr;
	Model* playerModel = nullptr;
	Model* groundModel = nullptr;
	Object3d* playerObj = nullptr;
	Object3d* groundObj = nullptr;
	int nowScene = 0;
	float playerScale = 1.0f;
	float groundScale = 15.0f;
	float largeCarScale = 3.0f;
	float scrollGround = 35.0f;
	float scrollCar = 60.0f;
	int scrollCount = 1;
	float nowTime = 0;
	float endTime = 1.0;
	float timeRate = 0;
	bool isChange = false;

	XMFLOAT3 frontMove;
	float backMove;
	float rightMove;
	float leftMove;
};