#include "GameScene.h"
#include <cassert>

GameScene::GameScene() {

}

GameScene::~GameScene() {
	safe_delete(playerModel);
	safe_delete(groundModel);
	safe_delete(bossModel);
	safe_delete(title);
	safe_delete(uiBack);
	safe_delete(gameOver);
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio) {
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->titleAudio = audio;
	this->playAudio = audio;
	this->gameoverAudio = audio;
	this->carAudio = audio;
	this->dangerAudio = audio;
	this->deadAudio = audio;
	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
	}
	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(1, L"Resources/warningMark.png")) {
		assert(0);
	}
	// 前景スプライト生成

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(2, L"Resources/title.png")) {
		assert(0);
	}
	// 前景スプライト生成
	//タイトル
	title = Sprite::Create(2, { 0.0f,0.0f });
	title->SetSize({ 1280.0f,960.0f });

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(13, L"Resources/uiBack.png")) {
		assert(0);
	}
	//回数
	uiBack = Sprite::Create(13, { 0.0f,0.0f });
	uiBack->SetSize({ 1280.0f,960.0f });

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(14, L"Resources/GameOver.png")) {
		assert(0);
	}
	//リザルト
	gameOver = Sprite::Create(14, { 0.0f,0.0f });
	gameOver->SetSize({ 1280.0f,960.0f });

	//.objの名前を指定してモデルを読み込む
	playerModel = playerModel->CreateFromObject("player");
	groundModel = groundModel->CreateFromObject("ground");
	bossModel = bossModel->CreateFromObject("boss");

	// 3Dオブジェクト生成
	playerObj = Object3d::Create();
	groundObj = Object3d::Create();
	bossObj = Object3d::Create();

	// 3Dオブジェクトにモデルを割り当てる

	playerObj->SetModel(playerModel);
	playerObj->SetPosition({ 25.0f, 0.0f, -25.0f });
	playerObj->SetScale({ 5.0f, 5.0f, 5.0f });
	playerObj->SetRotation({ 0, -45.0f, 0 });
	groundObj->SetModel(groundModel);
	groundObj->SetPosition({ 0.0f, -5.0f, 0.0f });
	groundObj->SetScale({ 5.0f, 5.0f, 5.0f });
	groundObj->SetRotation({ 0, 180.0f, 0 });
	bossObj->SetModel(bossModel);
	bossObj->SetPosition({ 0.0f, 0.0f, 0.0f });
	bossObj->SetScale({ 5.0f, 5.0f, 5.0f });
	bossObj->SetRotation({ 0, 135.0f, 0 });

	playerPos = playerObj->GetPosition();
	playerRot = playerObj->GetRotation();
	cameraEye = Object3d::GetEye();
	cameraTarget = Object3d::GetTarget();
}

void GameScene::Update() {
	Move();
	avoidance();
	collision();

	playerObj->Update();
	groundObj->Update();
	bossObj->Update();
}

void GameScene::resetPos() {
	playerObj->SetPosition({ 0.0f, 0.0f, 0.0f });
}

void GameScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();
	//XMFLOAT3 playerPos = playerObj->GetPosition();
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());
	// 背景スプライト描画

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion 背景スプライト描画
#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(dxCommon->GetCommandList());
	// 3Dオブクジェクトの描画
	playerObj->Draw();
	groundObj->Draw();
	bossObj->Draw();
	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion 3Dオブジェクト描画
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());
	// 前景スプライトの描画

	// デバッグテキストの描画
	debugText.DrawAll(dxCommon->GetCommandList());
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion 前景スプライト描画
}

void GameScene::Move() {
	// オブジェクト移動
	// 移動後の座標を計算
	if (avoidChange == 0) {
		if (input->PushKey(DIK_W)) {
			playerPos.z += moveAmount;
			cameraEye.z += moveAmount;
			cameraTarget.z += moveAmount;
			playerPos.x -= moveAmount;
			cameraEye.x -= moveAmount;
			cameraTarget.x -= moveAmount;
			playerRot.y = -45.0f;
			if (input->PushKey(DIK_D)) {
				playerPos.z += moveAmount/2;
				cameraEye.z += moveAmount/2;
				cameraTarget.z += moveAmount/2;
				playerPos.x += moveAmount;
				cameraEye.x += moveAmount;
				cameraTarget.x += moveAmount;
				playerRot.y = 0.0f;
			} else if (input->PushKey(DIK_A)) {
				playerPos.x -= moveAmount/2;
				cameraEye.x -= moveAmount/2;
				cameraTarget.x -= moveAmount/2;
				playerPos.z -= moveAmount;
				cameraEye.z -= moveAmount;
				cameraTarget.z -= moveAmount;
				playerRot.y = -90.0f;
			}
		} 
		else if (input->PushKey(DIK_S)) {
			playerPos.z -= moveAmount;
			cameraEye.z -= moveAmount;
			cameraTarget.z -= moveAmount;
			playerPos.x += moveAmount;
			cameraEye.x += moveAmount;
			cameraTarget.x += moveAmount;
			playerRot.y = 135.0f;
			if (input->PushKey(DIK_D)) {
				playerPos.x += moveAmount/2;
				cameraEye.x += moveAmount/2;
				cameraTarget.x += moveAmount/2;
				playerPos.z += moveAmount;
				cameraEye.z += moveAmount;
				cameraTarget.z += moveAmount;
				playerRot.y = 90.0f;
			} else if (input->PushKey(DIK_A)) {
				playerPos.z -= moveAmount/2;
				cameraEye.z -= moveAmount/2;
				cameraTarget.z -= moveAmount/2;
				playerPos.x -= moveAmount;
				cameraEye.x -= moveAmount;
				cameraTarget.x -= moveAmount;
				playerRot.y = 180.0f;
			}
		} 
		else if (input->PushKey(DIK_D)) {
			playerPos.x += moveAmount;
			cameraEye.x += moveAmount;
			cameraTarget.x += moveAmount;
			playerPos.z += moveAmount;
			cameraEye.z += moveAmount;
			cameraTarget.z += moveAmount;
			playerRot.y = 45.0f;
		} 
		else if (input->PushKey(DIK_A)) {
			playerPos.x -= moveAmount;
			cameraEye.x -= moveAmount;
			cameraTarget.x -= moveAmount;
			playerPos.z -= moveAmount;
			cameraEye.z -= moveAmount;
			cameraTarget.z -= moveAmount;
			playerRot.y = -135.0f;
		}
	}

	// 座標の変更を反映
	playerObj->SetPosition(playerPos);
	playerObj->SetRotation(playerRot);
	Object3d::SetEye(cameraEye);
	Object3d::SetTarget(cameraTarget);
}

void GameScene::avoidance() {
	if (input->TriggerKey(DIK_SPACE) && avoidChange == 0) {
		avoidChange = 1;
		startPlayerPos = playerPos;
		startCameraEye = cameraEye;
		startCameraTarget = cameraTarget;
		endPlayerPos = playerPos;
		endCameraEye = cameraEye;
		endCameraTarget = cameraTarget;
	}

	if (avoidChange != 0) {
		nowTime += 0.01;
		timeRate = min(nowTime / endTime, 1);
	}

	if (avoidChange == 1) {
		if (input->PushKey(DIK_W)) {
			endPlayerPos.z = startPlayerPos.z + avoidMove;
			endCameraEye.z = startCameraEye.z + avoidMove;
			endCameraTarget.z = startCameraTarget.z + avoidMove;
			endPlayerPos.x = startPlayerPos.x - avoidMove;
			endCameraEye.x = startCameraEye.x - avoidMove;
			endCameraTarget.x = startCameraTarget.x - avoidMove;
			if (input->PushKey(DIK_D)) {
				endPlayerPos.x = startPlayerPos.x;
				endCameraEye.x = startCameraEye.x;
				endCameraTarget.x = startCameraTarget.x;
			} else if (input->PushKey(DIK_A)) {
				endPlayerPos.z = startPlayerPos.z;
				endCameraEye.z = startCameraEye.z;
				endCameraTarget.z = startCameraTarget.z;
			}
		} else if (input->PushKey(DIK_S)) {
			endPlayerPos.z = startPlayerPos.z - avoidMove;
			endCameraEye.z = startCameraEye.z - avoidMove;
			endCameraTarget.z = startCameraTarget.z - avoidMove;
			endPlayerPos.x = startPlayerPos.x + avoidMove;
			endCameraEye.x = startCameraEye.x + avoidMove;
			endCameraTarget.x = startCameraTarget.x + avoidMove;
			if (input->PushKey(DIK_D)) {
				endPlayerPos.z = startPlayerPos.z;
				endCameraEye.z = startCameraEye.z;
				endCameraTarget.z = startCameraTarget.z;
			} else if (input->PushKey(DIK_A)) {
				endPlayerPos.x = startPlayerPos.x;
				endCameraEye.x = startCameraEye.x;
				endCameraTarget.x = startCameraTarget.x;
			}
		} else if (input->PushKey(DIK_D)) {
			endPlayerPos.z = startPlayerPos.z + avoidMove;
			endCameraEye.z = startCameraEye.z + avoidMove;
			endCameraTarget.z = startCameraTarget.z + avoidMove;
			endPlayerPos.x = startPlayerPos.x + avoidMove;
			endCameraEye.x = startCameraEye.x + avoidMove;
			endCameraTarget.x = startCameraTarget.x + avoidMove;
		} else if (input->PushKey(DIK_A)) {
			endPlayerPos.z = startPlayerPos.z - avoidMove;
			endCameraEye.z = startCameraEye.z - avoidMove;
			endCameraTarget.z = startCameraTarget.z - avoidMove;
			endPlayerPos.x = startPlayerPos.x - avoidMove;
			endCameraEye.x = startCameraEye.x - avoidMove;
			endCameraTarget.x = startCameraTarget.x - avoidMove;
		}
		avoidChange = 2;
	}
	if (avoidChange == 2) {
		playerPos = easeOutQuint(startPlayerPos, endPlayerPos, timeRate);
		cameraEye = easeOutQuint(startCameraEye, endCameraEye, timeRate);
		cameraTarget = easeOutQuint(startCameraTarget, endCameraTarget, timeRate);
	}

	// 座標の変更を反映
	playerObj->SetPosition(playerPos);
	Object3d::SetEye(cameraEye);
	Object3d::SetTarget(cameraTarget);

	if (timeRate == 1) {
		nowTime = 0;
		timeRate = 0;
		avoidChange = 0;
	}
}

void GameScene::collision() {

}