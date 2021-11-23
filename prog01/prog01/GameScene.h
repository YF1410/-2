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
	//攻撃
	void playerAttack();
	//当たり判定
	void collision();
	//boss攻撃
	void bossAttack();


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
	Model* bossModel = nullptr;
	Model* player_attack_befor_Model = nullptr;
	Model* player_attack_after_Model = nullptr;
	Model* player_dash_Model = nullptr;
	Model* boss_rush_befor_Model = nullptr;
	Model* boss_rush_after_Model = nullptr;

	Object3d* playerObj = nullptr;
	Object3d* groundObj = nullptr;
	Object3d* bossObj = nullptr;

	XMFLOAT3 playerPos;
	XMFLOAT3 playerRot;
	XMFLOAT3 startPlayerPos;
	XMFLOAT3 endPlayerPos;

	XMFLOAT3 cameraEye;
	XMFLOAT3 cameraTarget;
	XMFLOAT3 startCameraEye;
	XMFLOAT3 endCameraEye;
	XMFLOAT3 startCameraTarget;
	XMFLOAT3 endCameraTarget;

	XMFLOAT3 bossPos;
	XMFLOAT3 bossRot;
	XMFLOAT3 startBossPos;
	XMFLOAT3 endBossPos;

	int nowScene = 0;
	int charapose = 0;
	int bosspose = 0;

	bool attack_flag = false;
	bool boss_rush_flag = false;
	float bossframe = 0.0f;
	float frame = 0.0f;

	float playerScale = 1.0f;
	float groundScale = 15.0f;
	float largeCarScale = 3.0f;
	float scrollGround = 35.0f;
	float nowTime = 0;
	float endTime = 0.5;
	float timeRate = 0;
	int playerMode = 0;
	int rushChange = 0;
	float moveAmount = 1.0f;
	float avoidMove = 40.0f;
	float countDown = 60;
	float rushEndTime = 0;

	float playerRadius = 0;
	float bossRadius = 0;
	float collisionX = 0;
	float collisionZ = 0;
	float Collision = 0;
	bool collisionFlag = 0;

	int playerHp = 3;
	int bossHp = 3;
};